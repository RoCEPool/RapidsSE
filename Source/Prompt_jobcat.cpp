/*============================================================================*/
/* Prompt_jobcat.cpp -- print job catalog                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_jobcat (
  "Prompt_jobcat"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the functopm
  "jobcat"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_jobcat,      // prompt post-process function
  true                 // enabled
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_jobcat(SePrompt* pPrompt)
{
  JobClass_o::vJobCatHeader();
  
  i8 nJobs = 0;
  
  // thread
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_BOOT);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_PROMPT);

  // system
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_START);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_CONNECTOR);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_ANSWER);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_RECEIVER);

  // local
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_DOCOL);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_CF);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LF);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_PWF);
  
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTTENANTS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTCATALOGS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTSCHEMAS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTTABLES);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTCOLUMNS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTIDXCOLUMNS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTINDEXES);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTPARTCOLUMNS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LISTPARTITIONS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_USESCHEMA);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SHOWCLUSTER);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_DUMPINDEX);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_VISUALIZEINDEX);

  // generic
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB__BEGUN);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB__EXECUTE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB__EXECUTED);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB__END);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB__ENDED);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB__CANCEL);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB__CANCELLED);

  // dist2pc
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_ANALYZEINDEX);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_ANALYZETABLE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_COMPACTINDEX);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_CREATECATALOG);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_CREATEINDEX);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_CREATESCHEMA);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_CREATETABLE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_CREATETENANT);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_DROPCATALOG);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_DROPINDEX);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_DROPSCHEMA);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_DROPTABLE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_DROPTENANT);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LOADTABLE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_FETCHROWS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SIZEINDEX);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SIZETABLE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_TABLESIZES);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_TRUNCATETABLE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_VERIFYINDEX);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SANITY);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SPACE);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_STATUS);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_STARTCLUSTER);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_WAITCLUSTER);

  // child
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SUBLOADER);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SUBFETCH);
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_SUBTRUNCATE);
  
  // grankid
  nJobs++; JobCatalog::vTraceJobs_s(CN_JOB_LOADBUFFER);

  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WAITCLUSTER - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs\n\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
