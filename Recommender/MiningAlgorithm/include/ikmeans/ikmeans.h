#ifndef ICLUSTERING_H
#define ICLUSTERING_H

#ifdef HAVE_RECOMMENDER
#include "recommender/cb.h"
#include "recommender/random.h"
#else
#include "cb.h"
#include "random.h"
#endif

#include "vector"

using namespace std;

/**
 * Iterative Clustering with prediction of number of clusters.
 * Cluster method is k-means. Prediction of number of clusters
 * uses L-Method and BIC.
 * For a description of L-Method, see "Determining the
 * Number of Clusters/Segments in Hierarchical Clustering/
 * Segmentation Algorithms", by Stan Salvador and Philip Chan.
 * @author Marco Cristo and Angelo Filipe. 
 * K-means code and some auxiliary code by Marco Tuononen
 * (available at http://cs.joensuu.fi/~mtuonon/). 
 * BIC code by Qinpei Zhao (available at http://cs.joensuu.fi/~zhao/Software/).
 */
class IterativeKMeans
{
	private:
		vector<vector<int> > points;		/** Points (vectors) to be clustered */
		vector<int> clusterid;			/** Cluster ids assigned to each point */
		vector<vector<int> > centroids;		/** Cluster centroids */

		/**
 		 * Convert integer matrix to internal KMeans Format.
 		 * @param mat Pre-alocated integer matrix (int **m)
 		 * @param vecCount Number of lines in m (number of vectors)
 		 * @param vecLen Number of columns in m (vector dimension)
 		 */
		void matrix2CB(int **mat, int vecCount, int vecLen, CODEBOOK* CB);

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
		double RMSE(double *x, double *y, int start, int end, double a, double b);

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
		void getFunction(double *x, double *y, int start, int end, double *a, double *b);

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
		int LMethod(double *x, double *y, int N);

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
		int predictedNumberClusters(double *bicValues, int Minclus, int Maxclus);

	public:
		/**
		 * Constructor for IterativeKMeans. 
		 */
		IterativeKMeans(vector<vector<int> > points) { //angelo - retirei a referencia &
			this->points = points;
			initrandom(0);
		};

		/**
		 * Returns cluster ids found after last clustering. 
		 * @return Vector of cluster ids.
		 */
		vector<int> &getClusterIds() {
			return clusterid;
		};
			
		/**
		 * Returns centroids found after last clustering. 
		 * @return Vector of centroids.
		 */
		vector<vector<int> > &getCentroids() {
			return centroids;
		};		

		/**
		 * Cluster the vectors in points into k clusters. 
		 * @param k Number of clusters.
		 * @return True, if clustering succeded.
		 */
		bool cluster(int k);	

		/**
		 * Perform clustering on vectors in points, choosing the best partition from Minclus
		 * to Maxclus clusters. 
		 * @param Minclus Minimum number of clusters.
		 * @param Maxclus Maximum number of clusters.
		 * @return True, if clustering succeded.
		 */
		bool cluster(int Minclus, int Maxclus);	
};


#endif
