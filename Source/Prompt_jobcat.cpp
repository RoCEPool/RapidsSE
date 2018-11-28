/*============================================================================*/
/* Prompt_jobcat.cpp -- print job catalog                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_jobcat (
  "Prompt_jobcat"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the functopm
  "jobcat"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_jobcat,      // prompt function
  true,                // enabled

  true,                // visible
  "fun help"_Stz       // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_jobcat(SePrompt* pPrompt)
{
  JobClass::vJobCatHeader();
  
  i8 nJobs = 0;
  
  // thread
  nJobs++; JobClass::vTraceJobs(CN_JOB_BOOT);

  // system
  nJobs++; JobClass::vTraceJobs(CN_JOB_START);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CONNECTCLUSTER);
  nJobs++; JobClass::vTraceJobs(CN_JOB_NODEINFO);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CONNECTOR);
  nJobs++; JobClass::vTraceJobs(CN_JOB_ANSWER);
  nJobs++; JobClass::vTraceJobs(CN_JOB_RECEIVER);
  nJobs++; JobClass::vTraceJobs(CN_JOB_ALARM);
  nJobs++; JobClass::vTraceJobs(CN_JOB_PERIODIC);
  nJobs++; JobClass::vTraceJobs(CN_JOB_NETHOG);

  // local
  nJobs++; JobClass::vTraceJobs(CN_JOB_DOCOL);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CF);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LF);
  nJobs++; JobClass::vTraceJobs(CN_JOB_PWF);
  nJobs++; JobClass::vTraceJobs(CN_JOB_PING);

  nJobs++; JobClass::vTraceJobs(CN_JOB_EXPORT);
  nJobs++; JobClass::vTraceJobs(CN_JOB_GENERATE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_INGEST);
  nJobs++; JobClass::vTraceJobs(CN_JOB_INSERTROW);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTTENANTS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTCATALOGS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTSCHEMAS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTTABLES);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTCOLUMNS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTIDXCOLUMNS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTINDEXES);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTPARTCOLUMNS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LISTPARTITIONS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_USESCHEMA);
  nJobs++; JobClass::vTraceJobs(CN_JOB_SHOWCLUSTER);
  nJobs++; JobClass::vTraceJobs(CN_JOB_DUMPINDEX);
  nJobs++; JobClass::vTraceJobs(CN_JOB_ENDINSERT);
  nJobs++; JobClass::vTraceJobs(CN_JOB_VISUALIZEINDEX);

  // generic
  nJobs++; JobClass::vTraceJobs(CN_JOB__BEGUN);
  nJobs++; JobClass::vTraceJobs(CN_JOB__EXECUTE);
  nJobs++; JobClass::vTraceJobs(CN_JOB__EXECUTED);
  nJobs++; JobClass::vTraceJobs(CN_JOB__END);
  nJobs++; JobClass::vTraceJobs(CN_JOB__ENDED);
  nJobs++; JobClass::vTraceJobs(CN_JOB__CANCEL);
  nJobs++; JobClass::vTraceJobs(CN_JOB__CANCELLED);

  // dist2pc
  nJobs++; JobClass::vTraceJobs(CN_JOB_ANALYZEINDEX);
  nJobs++; JobClass::vTraceJobs(CN_JOB_ANALYZETABLE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_COMPACTINDEX);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CREATECATALOG);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CREATEINDEX);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CREATESCHEMA);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CREATETABLE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_CREATETENANT);
  nJobs++; JobClass::vTraceJobs(CN_JOB_DROPCATALOG);
  nJobs++; JobClass::vTraceJobs(CN_JOB_DROPINDEX);
  nJobs++; JobClass::vTraceJobs(CN_JOB_DROPSCHEMA);
  nJobs++; JobClass::vTraceJobs(CN_JOB_DROPTABLE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_DROPTENANT);
  nJobs++; JobClass::vTraceJobs(CN_JOB_ENDINSERT);
  nJobs++; JobClass::vTraceJobs(CN_JOB_LOADTABLE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_FETCHROWS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_SIZEINDEX);
  nJobs++; JobClass::vTraceJobs(CN_JOB_SIZETABLE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_TABLESIZES);
  nJobs++; JobClass::vTraceJobs(CN_JOB_TRUNCATETABLE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_VERIFYINDEX);
  nJobs++; JobClass::vTraceJobs(CN_JOB_SANITY);
  nJobs++; JobClass::vTraceJobs(CN_JOB_SPACE);
  nJobs++; JobClass::vTraceJobs(CN_JOB_STATUS);
  nJobs++; JobClass::vTraceJobs(CN_JOB_STARTCLUSTER);
  nJobs++; JobClass::vTraceJobs(CN_JOB_WAITCLUSTER);

  // child
  nJobs++; JobClass::vTraceJobs(CN_JOB_SUBLOADER);
  nJobs++; JobClass::vTraceJobs(CN_JOB_SUBFETCH);
  nJobs++; JobClass::vTraceJobs(CN_JOB_SUBTRUNCATE);
  
  // grankid
  nJobs++; JobClass::vTraceJobs(CN_JOB_LOADBUFFER);
  nJobs++; JobClass::vTraceJobs(CN_JOB_WRITEBUFFER);
  
  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WRITEBUFFER - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs (%lld v. %lld\n\n"_Stz,
           nJobs, CN_JOB_WRITEBUFFER - CN_JOB__BEGUN + 1);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
