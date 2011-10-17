/*
$Revision: 1.3 $
$Date: 2005/06/27 11:01:03 $
$Author: mtuonone $
$Name:  $
$Id: reporting.c,v 1.3 2005/06/27 11:01:03 mtuonone Exp $
*/

/*-------------------------------------------------------------------*/
/* REPORTING.C     Marko Tuononen                                    */
/*                                                                   */
/* Provides reporting (time and error) facilities.                   */
/*                                                                   */
/*-------------------------------------------------------------------*/

#define ProgName       "REPORTING"
#define VersionNumber  "Version 0.20"
#define LastUpdated    "09.03.2005"

/*-------------------------------------------------------------------*/

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_RECOMMENDER
#include "recommender/cb.h"
#include "recommender/file.h"
#include "recommender/reporting.h"
#include "recommender/interfc.h"
#else
#include "../../include/ikmeans/cb.h"
#include "../../include/ikmeans/file.h"
#include "../../include/ikmeans/reporting.h"
#include "../../include/ikmeans/interfc.h"
#endif


/* =================== FUNCTIONS ==================================== */

double SetClock(double* start)
{  
  struct timeval tv;  
  gettimeofday(&tv, NULL);

  return( *start = (double)tv.tv_sec + ((double)tv.tv_usec /1e6) );
}  /* SetClock() */

/* ------------------------------------------------------------------ */

double GetClock(double start)
{
  struct timeval tv;
  double elapsed;

  gettimeofday(&tv, NULL);
  elapsed = (double)tv.tv_sec + ((double)tv.tv_usec /1e6) - start;

  return elapsed;
}  /* GetClock() */

/* ------------------------------------------------------------------ */

void PrintHeader(int quietLevel)
{
  if ((quietLevel >= 2))
  {
    PrintMessage("\n");
    PrintMessage("Iteration\tMSE\t\tTime\n");
  }
}  /* PrintHeader() */

/* ------------------------------------------------------------------ */

void PrintIterationKM(int quietLevel, int i, int iter, double error,
double time)
{
  if ((quietLevel >= 2) && (i == 0))
  {
    PrintMessage("%d\t\t%f\t%f\n", iter, error, time);
  }
}  /* PrintIterationKM() */

/* ------------------------------------------------------------------ */

void PrintIterationRLS(int quietLevel, int iter, double error, double time, 
int better)
{
  if ((quietLevel >= 3) || ((quietLevel >= 2) && better))
  {
    PrintMessage("%d\t\t%f\t%f", iter, error, time);

    if (better)
    {
      PrintMessage(" *\n");
    }
    else
    {
      PrintMessage("\n");
    }
  }
}  /* PrintIterationRLS() */

/* ------------------------------------------------------------------ */

void PrintRepeat(int quietLevel, int repeats, int i, int iter, double error,
double time, int better)
{
  char str[127];

  sprintf(str, "Repeat %d:\t%f\t%f\t(%d iterations)", i+1, error, time, iter);

  if ((quietLevel >= 2) && (repeats > 1))
  {
    if (i == 0)
    {
      PrintMessage("%s *\n\n", str);
    }
    else if (better)
    {
      PrintMessage("%s *\n", str);
    }
    else if (quietLevel >= 3)
    {
      PrintMessage("%s\n", str);
    }
  }
}  /* PrintRepeat() */

/* ------------------------------------------------------------------ */

void PrintFooterKM(int quietLevel, double error, int repeats,
double totalTime, int totalIter)
{
  if (quietLevel >= 2)
  {
    PrintMessage("\nmse = %f  time = %f  %d repeats  %d iterations\n\n", error, totalTime, repeats, totalIter);
  }
  else if (quietLevel == 1)
  {
    PrintMessage("%f\t%f\n", error, totalTime);
  }
}  /* PrintFooterKM() */

/* ------------------------------------------------------------------ */

void PrintFooterRLS(int quietLevel, int iter, double error, double time)
{
  if (quietLevel >= 2)
  {
    PrintMessage("\nmse = %f  time = %f  %d iterations\n\n", error, time, iter);
  }
  else if (quietLevel == 1)
  {
    PrintMessage("%f\t%f\n", error, time);
  }
}  /* PrintFooterRLS() */

/* ------------------------------------------------------------------ */

int DetermineFileName(char *name)
{
  char newName[MAXFILENAME], suffix[MAXFILENAME];
  int  i;
  
  /* Without extension */
  if (ExistFile(name))
  {
    return 1;
  }
 
  for (i = 0; i < 3; i++)
  {
    if (i == 0)      /* Try TS-file extension */
      strcpy(suffix, FormatNameTS);
      
    else if (i == 1)  /* Try CB-file extension */
      strcpy(suffix, FormatNameCB);
      
    else             /* Try PA-file extension */
      strcpy(suffix, FormatNamePA);
    
    if (strlen(name) < MAXFILENAME-strlen(suffix)-1)
    {
      strcpy(newName, name);
      CheckFileName(newName, suffix);
      if (ExistFile(newName))
      {
        strcpy(name, newName);
        return 1;
      }
    }
  }

  /* No luck this time */
  return 0;
}  /* DetermineFileName() */
 
/* ------------------------------------------------------------------ */
 //modify by QP on 2k8-4-7
//add two parameters 
//int dis; int cri; for distance function and criteria function seperately
TRAININGSET CheckParameters(char *TSName, char *CBName, char *PAName, 
char *InName, int Minclus,int Maxclus, int ow) {
  TRAININGSET TS;
  
  /* input training set doesn't exist */
  if (!ExistFile(TSName)) 
  {
    ErrorMessage("\nERROR: Input training set doesn't exist: "
        "%s\n\n", TSName);
    ExitProcessing(FATAL_ERROR);
  }
    
  /* result codebook file exists and we are told not to overwrite */
  if (ExistFile(CBName) && !ow) 
  {
    ErrorMessage("\nERROR: Result codebook already exists: "
        "%s\n\n", CBName);
    ExitProcessing(FATAL_ERROR);
  }

  /* result partitioning file exists and we are told not to overwrite */
  if (*PAName && ExistFile(PAName) && !ow) 
  {
    ErrorMessage("\nERROR: Result partitioning already exists: "
        "%s\n\n", PAName);
    ExitProcessing(FATAL_ERROR);
  }
  
  /* initial codebook / partitioning doesn't exist */
  if (*InName && !DetermineFileName(InName))
  {
    ErrorMessage("\nERROR: Initial codebook/partitioning doesn't exist: %s\n\n", InName);
    ExitProcessing(FATAL_ERROR);
  }
   //add by QP on 2k8-4-7
  //verify whether Max is bigger than min; make sure the range is ok
    if (Maxclus < Minclus ) 
	{
		ErrorMessage("Bad range: %i < %i.\n", Maxclus, Minclus);
		ExitProcessing(FATAL_ERROR);
    } 
  
  ReadTrainingSet(TSName, &TS);

 
  //add by QP on 2k7-11-7  
  //the size of training set should be at least more than min codebook
  /* result codebook cannot contain more vectors than training set */
  if (BookSize(&TS) < Minclus)
  {
    ErrorMessage("\nERROR: Number of vectors in training set ");
    ErrorMessage("(%d) < Min number of clusters ", BookSize(&TS));
    ErrorMessage("(%d%d)!\n\n", Minclus, Maxclus);
    FreeCodebook(&TS);
    ExitProcessing(FATAL_ERROR);
  }
  
  return TS;
}  /* CheckParameters() */

/* ------------------------------------------------------------------ */
//modify by QP on 2k8-4-7 
//change the function ReadInitialCBorPA( )
//add two parameters int Minclus and Maxclus,delete int clus
//Minclus -- Min number of clusters
//Maxclus -- Max number of clusters
int ReadInitialCBorPA(char *InName, int Minclus, int Maxclus, TRAININGSET *pTS, 
CODEBOOK *pCB, PARTITIONING *pP) 
{
  int useInitial = 0;
  
  if (*InName)  /* we use initial codebook/partitioning */
  {
    switch (DetermineCBFileType(InName)) {
      case TSFILE: case CBFILE:
        ReadCodebook(InName, pCB);
        useInitial = 1;
        //modify by QP on 2k8-4-7 
		//If we use initial ones, we just need to judge the size of codebook/
		//training set is in the range: [Minclus, Maxclus]
		if (BookSize(pCB) < Minclus || BookSize(pCB) > Maxclus )
        {
          ErrorMessage("\nERROR: Number of vectors in initial codebook ");
          ErrorMessage("(%d) <> number of clusters ", BookSize(pCB));
          ErrorMessage("(%d)(%d)!\n\n", Minclus, Maxclus);
          FreeCodebook(pTS);
          FreeCodebook(pCB);
          ExitProcessing(FATAL_ERROR);
        }
        
        CreateNewPartitioning(pP, pTS, BookSize(pCB));
        break;
        
      case PAFILE:
        ReadPartitioning(InName, pP, pTS);
        useInitial = 2;
        
        //modify by QP on 2k8-4-7 
		//judge the size of partitioning is in the range: [Minclus, Maxclus]
        if (PartitionCount(pP)<Minclus || PartitionCount(pP) > Maxclus)
        {
          ErrorMessage("\nERROR: Number of partitions in initial partitioning ");
          ErrorMessage("(%d) <> number of clusters ", PartitionCount(pP));
          ErrorMessage("(%d)(%d)!\n\n", Minclus, Maxclus);
          FreeCodebook(pTS);
          FreePartitioning(pP);
          ExitProcessing(FATAL_ERROR);
        }
        
        CreateNewCodebook(pCB, PartitionCount(pP), pTS);
        break;
        
      case NOTFOUND:
        ErrorMessage("\nERROR: Type of initial codebook/partitioning file "
            "%s is unidentified!\n\n", InName);
        FreeCodebook(pTS);
        ExitProcessing(FATAL_ERROR);
        break;
    }      
  }
  else  /* we don't use initial codebook/partitioning */
  {
 //   CreateNewCodebook(pCB, clus, pTS);
 //   CreateNewPartitioning(pP, pTS, clus);
    useInitial = 0;
  }
  
  return useInitial;
}

/* ------------------------------------------------------------------ */
