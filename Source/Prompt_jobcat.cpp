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
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_BOOT);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_PROMPT);

  // system
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_START);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_CONNECTOR);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_ANSWER);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_RECEIVER);

  // local
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_DOCOL);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_CF);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LF);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_PWF);
  
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTTENANTS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTCATALOGS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTSCHEMAS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTTABLES);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTCOLUMNS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTIDXCOLUMNS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTINDEXES);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTPARTCOLUMNS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LISTPARTITIONS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_USESCHEMA);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SHOWCLUSTER);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_DUMPINDEX);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_VISUALIZEINDEX);

  // generic
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB__BEGUN);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB__EXECUTE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB__EXECUTED);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB__END);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB__ENDED);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB__CANCEL);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB__CANCELLED);

  // dist2pc
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_ANALYZEINDEX);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_ANALYZETABLE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_COMPACTINDEX);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_CREATECATALOG);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_CREATEINDEX);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_CREATESCHEMA);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_CREATETABLE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_CREATETENANT);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_DROPCATALOG);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_DROPINDEX);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_DROPSCHEMA);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_DROPTABLE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_DROPTENANT);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LOADTABLE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_FETCHROWS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SIZEINDEX);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SIZETABLE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_TABLESIZES);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_TRUNCATETABLE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_VERIFYINDEX);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SANITY);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SPACE);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_STATUS);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_STARTCLUSTER);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_WAITCLUSTER);

  // child
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SUBLOADER);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SUBFETCH);
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_SUBTRUNCATE);
  
  // grankid
  nJobs++; JobClass_s::vTraceJobs_s(CN_JOB_LOADBUFFER);

  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WAITCLUSTER - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs\n\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
