/*
$Revision: 1.3 $
$Date: 2005/06/27 11:01:03 $
$Author: mtuonone $
$Name:  $
$Id: reporting.h,v 1.3 2005/06/27 11:01:03 mtuonone Exp $
*/

#if ! defined(__REPORTING_H)
#define __REPORTING_H

#define MAXFILENAME     40    /* maximum length of filename */

double SetClock(double* start);

double GetClock(double start);

void PrintHeader(int quietLevel);

void PrintIterationKM(int quietLevel, int i, int iter, double error,
    double time);

void PrintIterationRLS(int quietLevel, int iter, double error, double time,
    int better);

void PrintRepeat(int quietLevel, int repeats, int i, int iter, double error,
    double time, int better);

void PrintFooterKM(int quietLevel, double error, int repeats,
    double totalTime, int totalIter);

void PrintFooterRLS(int quietLevel, int iter, double error, double time);

int DetermineFileName(char *name);

TRAININGSET CheckParameters(char *TSName, char *CBName, char *PAName, 
    char *InName, int Minclus,int Maxclus, int ow);

int ReadInitialCBorPA(char *InName, int Minclus,int Maxclus, TRAININGSET *pTS, 
    CODEBOOK *pCB, PARTITIONING *pP);

#endif /* __REPORTING_H */

