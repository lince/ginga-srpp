/**
* Test of iterative clustering class
* @author Marco Cristo and Angelo Filipe. 
*/

#include "iostream"
#include "fstream"
#include "vector"
#include "stdlib.h"

#ifdef HAVE_RECOMMENDER
#include "recommender/ikmeans.h"
#else
#include "../../include/ikmeans/ikmeans.h"
#endif

using namespace std;

/* ===========================  TEST  ================================ */

int main(int argc, char* argv[])
{
  	int Minclus = 0;
  	int Maxclus = 0;
	int i = 0, j = 0;
  
	if (argc != 4) {
		cout << argv[0] << " vetores.txt minClusters maxClusters" << endl;
		return 1;
	}
	Minclus = atoi(argv[2]);
	Maxclus = atoi(argv[3]);
	if (Minclus >= Maxclus)	
		return 1; 

	vector<vector<int> > points;
	ifstream ifile(argv[1]);
	int dim, value, countvec = 0;
	if (ifile.is_open()) {
		ifile >> dim;
		ifile >> value;
		while (! ifile.eof()) {
			vector<int> values;
			values.push_back(value);
			for (int i=1; i<dim; i++) {
				ifile >> value;
				values.push_back(value);
			}	
			points.push_back(values);
			countvec ++;
			ifile >> value;
		}
		ifile.close();
	} else {
		cout << "Unable to open file" << endl;
		return 1;
	}

	IterativeKMeans ikm(points);
	ikm.cluster(Minclus, Maxclus);
	vector<int> clusterid = ikm.getClusterIds();
	vector<vector<int> > centroids = ikm.getCentroids();	

	for (i = 0; i < (int)clusterid.size(); i++)
		cout << i << " in cluster " << clusterid[i] << endl;	

	for (i = 0; i < (int)centroids.size(); i++) {
		cout << i << ": ";
		for (j = 0; j < (int)centroids[0].size(); j++)
			cout << (centroids[i])[j] << " ";	
		cout << endl;
	}

  	return 1;  
}  /* main() */


