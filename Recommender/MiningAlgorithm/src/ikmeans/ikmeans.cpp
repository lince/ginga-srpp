#include "iostream"
#include "fstream"
#include "vector"

#include "math.h"
#include "limits.h"
#include "string.h"

#ifdef HAVE_RECOMMENDER
#include "recommender/cb.h"
#include "recommender/file.h"
#include "recommender/interfc.h"
#include "recommender/memctrl.h"
#include "recommender/random.h"
#include "recommender/reporting.h"
#include "recommender/kmeans.h"
#include "recommender/ikmeans.h"
#else
#include "../../include/ikmeans/cb.h"
#include "../../include/ikmeans/file.h"
#include "../../include/ikmeans/interfc.h"
#include "../../include/ikmeans/memctrl.h"
#include "../../include/ikmeans/random.h"
#include "../../include/ikmeans/reporting.h"
#include "../../include/ikmeans/kmeans.h"
#include "../../include/ikmeans/ikmeans.h"
#endif

using namespace std;

/* ===========================  Conversion to CB Format  ================================ */

/**
 * Convert integer matrix to internal KMeans Format.
 * @param mat Pre-alocated integer matrix (int **m)
 * @param vecCount Number of lines in m (number of vectors)
 * @param vecLen Number of columns in m (vector dimension)
 */
void IterativeKMeans::matrix2CB(int **mat, int vecCount, int vecLen, CODEBOOK* CB)
{
  BOOKNODE *v;
  int   i, j, vsum;

  strcpy(CB->Versionstr, "VQ CODEBOOK 2.0\n");
  CB->BlockSizeX = vecLen;
  CB->BlockSizeY = 1;
  CB->CodebookSize = vecCount;
  CB->TotalFreq = vecCount;
  CB->BytesPerElement = sizeof(int); // maximo = 4 bytes
  CB->MinValue = INT_MIN;
  CB->MaxValue = INT_MAX;
  CB->Preprocessing = 0;
  strcpy(CB->GenerationMethod, "");

  AllocateMemoryForBook(CB);

  for(i=0; i<vecCount; i++)
  {
	v = &Node(CB,i);
  	v->name = NULL;

  	/* read vector elements */
  	vsum=0;
  	for( j=0; j<vecLen; j++ )
   	{
   		v->vector[j] = (VECTORELEMENT) mat[i][j];
   		vsum += v->vector[j];
   	}

  	/* read vector frequency */
  	v->freq = 1;

  	/* calculate vector mean */
  	v->vmean = (VECTORELEMENT) ( (double) vsum / vecLen + 0.5);
  }
}

/* ===========================  ALGORITHM TO FIND NUMBER OF CLUSTERS  ================================ */

/**
 * Returns root mean squared error between function ax + b
 * and set of points (x,y) in interval [start,end).
 * @param x Vector of x coordinates.
 * @param y Vector of y coordinates.
 * @param start Interval start.
 * @param end Interval end.
 * @param a Function slope.
 * @param b Function intercept.
 * @return Root mean squared error.
 */
double IterativeKMeans::RMSE(double *x, double *y, int start, int end, double a, double b)
{
	double ssdiff = 0.0;
	int i;
	for (i = start; i < end; i ++) 
		ssdiff += ((a + b * x[i]) - y[i]) * ((a + b * x[i]) - y[i]); 
	return sqrt(ssdiff / (end - start));
}

/**
 * Finds slope a and intercept b of best fit function of a set of points (x,y) 
 * in interval [start,end).
 * @param x Vector of x coordinates.
 * @param y Vector of y coordinates.
 * @param start Interval start.
 * @param end Interval end.
 * @param a Function slope.
 * @param b Function intercept.
 * @return Point which corresponds to line "knee".
 */
void IterativeKMeans::getFunction(double *x, double *y, int start, int end, double *a, double *b)
{
	double Sx = 0.0;
	double Sy = 0.0;
	double Sxy = 0.0;
	double Sx2 = 0.0;
	int i;
	int N = (end - start);

	for (i = start; i < end; i ++) {
		Sx += x[i];
		Sx2 += x[i] * x[i];
		Sy += y[i];
		Sxy += x[i] * y[i];
	}
	*b = ((double)N * Sxy - Sx * Sy) / ((double)N * Sx2 - Sx * Sx);
	*a = (Sy - (*b) * Sx) / N;	
}

/**
 * Finds "knee" of a line (given by its points [x,y]) using L-method.
 * For a description of L-Method, see "Determining the
 * Number of Clusters/Segments in Hierarchical Clustering/
 * Segmentation Algorithms", by Stan Salvador and Philip Chan.
 * @param x Vector of x coordinates.
 * @param y Vector of y coordinates.
 * @param N Number of points.
 * @return Point which corresponds to line "knee".
 */
int IterativeKMeans::LMethod(double *x, double *y, int N)
{
	double f1 = 0, f2 = 0, RMSE1 = 0, RMSE2 = 0, RMSEc = 0, a = 0, b = 0, min = 0;
	int minc = 0, c;

	for (c = 2; c < N-1; c ++) {
		f1 = ((double)c - 1) / (N - 1);
		if (f1 > 0) {
			getFunction(x, y, 0, c, &a, &b);
			RMSE1 = RMSE(x, y, 0, c, a, b);
		};
		f2 = ((double)(N - c)) / (N - 1);
		if (f2 > 0) {
			getFunction(x, y, c-1, N, &a, &b);
			RMSE2 = RMSE(x, y, c-1, N, a, b);
		}
		RMSEc = f1 * RMSE1 + f2 * RMSE2;
		// printf("c=%d #c=%f RMSEc=%f\n", c-1, x[c-1], RMSEc);
		if (RMSEc < min || c == 2) {
			min = RMSEc;
			minc = c-1;
		}
	}
	return minc;
}

/**
 * Predicts the number of clusters considering the BIC values associated
 * with several different partitions. The bicValues correspond to 
 * partitions from Minclus to Maxclus clusters. This method can only
 * be applied to a set with at least 4 partitions (4 points in the line 
 * function).
 * @param bicValues Vector of BIC values.
 * @param Minclus Minimum number of clusters.
 * @param Maxclus Maximum number of clusters.
 * @return Number of clusters.
 */
int IterativeKMeans::predictedNumberClusters(double *bicValues, int Minclus, int Maxclus)
{
	int n = (Maxclus - Minclus) + 1;
	if (n < 4) {
		// L-method requires 4 points to be applied
		// since it has to find two functions and each
		// function requires at least two points
		if (n == 0)
			return Minclus; 
		else
			return Minclus + 1; 
	}

	int nc = 0, i;
	double *x = (double *)malloc(n * sizeof(double));
	double *y = bicValues;

	for (i = 0; i < n; i ++)
		x[i] = i + Minclus; 

	nc = LMethod(x, y, n);
	nc = (int)x[nc];
	free(x);
	return nc;
};

/* ===========================  CLUSTER INTERFACE  ================================ */

/**
 * Cluster the vectors in points into k clusters. 
 * @param k Number of clusters.
 * @return True, if clustering succeded.
 */
bool IterativeKMeans::cluster(int k)
{
  	TRAININGSET   	TS;
  	CODEBOOK      	CB;
  	PARTITIONING  	P;
  	int           	i = 0, j = 0;
	bool 		success = true;
  
	// Setup
	int countvec = points.size();
	int dim = points[0].size();
	int **mat = new int * [countvec];
	for (int i = 0; i < countvec; i++) {
		mat[i] = new int [dim];
		for (int j = 0; j < dim; j ++) 
			mat[i][j] = (points[i])[j];
        }
	matrix2CB(mat, countvec, dim, &TS);
  
	CreateNewCodebook(&CB, k, &TS);
	CreateNewPartitioning(&P, &TS, k); 
	PerformKMeans(&TS, &CB, &P, k, 1, 2, 0);  // get solution

	// cluster ids
	clusterid.clear();
	for (i = 0; i < BookSize(&TS); i ++)
		clusterid.push_back(Map(&P, i));

	// centroids
	BOOKNODE *v;
	centroids.clear();
	for (i = 0; i < BookSize(&CB); i ++) {
		vector<int> vc;
		v = &Node(&CB,i);
		for(j = 0; j < CB.BlockSizeX; j ++ )
			vc.push_back(v->vector[j]);
		centroids.push_back(vc);
	}

  	FreeCodebook(&TS);
  	FreeCodebook(&CB);
  	FreePartitioning(&P);
 
	// Cleanup
	for (i = 0; i < countvec; i++)
		delete[] mat[i];
	delete[] mat;

  	return success;  
};


/**
 * Perform clustering on vectors in points, choosing the best partition from Minclus
 * to Maxclus clusters. 
 * @param Minclus Minimum number of clusters.
 * @param Maxclus Maximum number of clusters.
 * @return True, if clustering succeded.
 */
bool IterativeKMeans::cluster(int Minclus, int Maxclus)
{
  	TRAININGSET   	TS;
  	CODEBOOK      	CB;
  	PARTITIONING  	P;
  	int           	i = 0, j = 0;
  	double		ValidityIndice = 0;
  	double*		bicValues = NULL;
	bool 		success = true;
  
	// Setup
	int countvec = points.size();
	int dim = points[0].size();
	int **mat = new int * [countvec];
	for (int i = 0; i < countvec; i++) {
		mat[i] = new int [dim];
		for (int j = 0; j < dim; j ++) 
			mat[i][j] = (points[i])[j];
        }
	matrix2CB(mat, countvec, dim, &TS);
  
	CreateNewCodebook(&CB, Maxclus, &TS);
	CreateNewPartitioning(&P, &TS, Maxclus); 
	DecreaseCodebookSize(&CB, Minclus-1);
	DecreaseNumberOfPartitions(&P, Minclus-1);		

	/* cout << "# = " << countvec << endl;
	cout << "BKS = " << BookSize(&TS) << endl;
	cout << "CLUSTER min " << Minclus << endl;
	cout << "CLUSTER max " << Maxclus << endl; */
	
	bicValues = (double *) malloc((Maxclus - Minclus + 1) * sizeof(double));
	for(i = Minclus; i<= Maxclus; i++)
	{
			//allocate the size of CB/PA with the change of number of clusters
			IncreaseCodebookSize(&CB, i);
			IncreaseNumberOfPartitions(&P, i);
			if (PerformKMeans(&TS, &CB, &P, i, 1, 2, 0) != 0) {
				ErrorMessage("\nERROR: Clustering failed!\n\n");
				success = false;
				break;
			}

			ValidityIndice = ValidityBIC(&TS, &CB, &P);
			bicValues[i - Minclus] = ValidityIndice;
			// PrintMessage("cluster number = %d; Validity = %f\n", i, ValidityIndice);
	}

	if (success) {
		int nclusters = predictedNumberClusters(bicValues, Minclus, Maxclus);
		// PrintMessage("predicted number of clusters = %d\n", nclusters);
		if (nclusters != Maxclus) {
  			FreeCodebook(&CB);
  			FreePartitioning(&P);
			CreateNewCodebook(&CB, nclusters, &TS);
			CreateNewPartitioning(&P, &TS, nclusters); 
			PerformKMeans(&TS, &CB, &P, nclusters, 1, 2, 0);  // get solution
		}
		// cluster ids
		clusterid.clear();
		for (i = 0; i < BookSize(&TS); i ++)
			clusterid.push_back(Map(&P, i));

		// centroids
  		BOOKNODE *v;
		centroids.clear();
		for (i = 0; i < BookSize(&CB); i ++) {
			vector<int> vc;
			v = &Node(&CB,i);
  			for(j = 0; j < CB.BlockSizeX; j ++ )
				vc.push_back(v->vector[j]);
			centroids.push_back(vc);
		}
	}

	free(bicValues);
  	FreeCodebook(&TS);
  	FreeCodebook(&CB);
  	FreePartitioning(&P);
 
	// Cleanup
	for (i = 0; i < countvec; i++)
		delete[] mat[i];
	delete[] mat;

  	return success;  
};

