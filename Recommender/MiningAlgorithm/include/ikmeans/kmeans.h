/*
$Revision: 1.2 $
$Date: 2005/06/27 11:18:17 $
$Author: mtuonone $
$Name:  $
$Id: kmeans.h,v 1.2 2005/06/27 11:18:17 mtuonone Exp $
*/

#if ! defined(__KMEANS_H)
#define __KMEANS_H


int PerformKMeans(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP,
    int clus, int repeats, int quietLevel, int useInitial);
void SelectRandomRepresentatives(TRAININGSET *pTS, CODEBOOK *pCB);

void CalculateDistances(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP,
    llong *distance); //, DISTANCETYPE Dis

void OptimalRepresentatives(PARTITIONING *pP, TRAININGSET *pTS,
    CODEBOOK *pCB, int *active, int *activeCount);

void OptimalPartition(CODEBOOK *pCB, TRAININGSET *pTS, PARTITIONING *pP,
    int *active, int activeCount, llong *distance);//, DISTANCETYPE Dis

char* KMeansInfo(void);

//add by QP on 2k7-11-22
//add a function for calculating clustering validity
double ValidityDunn(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP);
// add by QP on 2K7-12-4
// F-ratio criteria
double ValidityFratio(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP);
// add by QP on 2k7-12-5
// DBI criteria
double ValidityDBI(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP);
//add by QP on 2k7-12-11
double ValidityMSE(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP);
//add by QP on 2k7-1-5 
//another implementation of F-ratio i.e. f-test from ISMO
double FTest(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP);
// add by QP on 2k7-1-10
// Baysian Information Criteria
double ValidityBIC(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP);
// add by QP on 2k7-1-14
double Xie_Beni(TRAININGSET *pTS, CODEBOOK *pCB, PARTITIONING *pP);

// add by QP on 2k7-1-14
// the function for getting mean vector of total dataset
VECTORTYPE MeanOfTs(TRAININGSET *pTS, CODEBOOK *pCB);

#endif /* __KMEANS_H */


