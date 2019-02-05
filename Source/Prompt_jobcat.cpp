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
  nJobs++; JobClass::vTraceJob(CN_JOB_BOOT);

  // info
  nJobs++; JobClass::vTraceJob(CN_JOB_NODEINFO);
  
  // system
  nJobs++; JobClass::vTraceJob(CN_JOB_ALARM);
  nJobs++; JobClass::vTraceJob(CN_JOB_ANSWER);
  nJobs++; JobClass::vTraceJob(CN_JOB_CONNECTOR);
  nJobs++; JobClass::vTraceJob(CN_JOB_PERIODIC);
  nJobs++; JobClass::vTraceJob(CN_JOB_RECEIVER);
  nJobs++; JobClass::vTraceJob(CN_JOB_START);

  // local
  nJobs++; JobClass::vTraceJob(CN_JOB_CAT);
  nJobs++; JobClass::vTraceJob(CN_JOB_CF);
  nJobs++; JobClass::vTraceJob(CN_JOB_COPYBUFFER);
  nJobs++; JobClass::vTraceJob(CN_JOB_DOCOL);
  nJobs++; JobClass::vTraceJob(CN_JOB_DUMPINDEX);
  nJobs++; JobClass::vTraceJob(CN_JOB_FETCHROWS);
  nJobs++; JobClass::vTraceJob(CN_JOB_GENERATE);
  nJobs++; JobClass::vTraceJob(CN_JOB_INGEST);
  nJobs++; JobClass::vTraceJob(CN_JOB_LF);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTCATALOGS);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTCOLUMNS);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTIDXCOLUMNS);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTINDEXES);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTPARTCOLUMNS);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTSCHEMAS);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTTABLES);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTTENANTS);
  nJobs++; JobClass::vTraceJob(CN_JOB_MONITOR);
  nJobs++; JobClass::vTraceJob(CN_JOB_PING);
  nJobs++; JobClass::vTraceJob(CN_JOB_PWF);
  nJobs++; JobClass::vTraceJob(CN_JOB_REPEATER);
  nJobs++; JobClass::vTraceJob(CN_JOB_REPORT);
  nJobs++; JobClass::vTraceJob(CN_JOB_SELECTTABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_SHOWCLUSTER);
  nJobs++; JobClass::vTraceJob(CN_JOB_SINK);
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBINSERTER);
  nJobs++; JobClass::vTraceJob(CN_JOB_USESCHEMA);
  nJobs++; JobClass::vTraceJob(CN_JOB_VISUALIZEINDEX);
 
  // generic
  nJobs++; JobClass::vTraceJob(CN_JOB__BEGUN);
  nJobs++; JobClass::vTraceJob(CN_JOB__CANCEL);
  nJobs++; JobClass::vTraceJob(CN_JOB__CANCELLED);
  nJobs++; JobClass::vTraceJob(CN_JOB__END);
  nJobs++; JobClass::vTraceJob(CN_JOB__ENDED);
  nJobs++; JobClass::vTraceJob(CN_JOB__EXECUTE);
  nJobs++; JobClass::vTraceJob(CN_JOB__EXECUTED);
  nJobs++; JobClass::vTraceJob(CN_JOB__FIND);

  // dist2pc
  nJobs++; JobClass::vTraceJob(CN_JOB_ANALYZEINDEX);
  nJobs++; JobClass::vTraceJob(CN_JOB_ANALYZETABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_COMPACTINDEX);
  nJobs++; JobClass::vTraceJob(CN_JOB_CONNECTCLUSTER);
  nJobs++; JobClass::vTraceJob(CN_JOB_CREATECATALOG);
  nJobs++; JobClass::vTraceJob(CN_JOB_CREATEINDEX);
  nJobs++; JobClass::vTraceJob(CN_JOB_CREATESCHEMA);
  nJobs++; JobClass::vTraceJob(CN_JOB_CREATETABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_CREATETENANT);
  nJobs++; JobClass::vTraceJob(CN_JOB_DROPCATALOG);
  nJobs++; JobClass::vTraceJob(CN_JOB_DROPINDEX);
  nJobs++; JobClass::vTraceJob(CN_JOB_DROPSCHEMA);
  nJobs++; JobClass::vTraceJob(CN_JOB_DROPTABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_DROPTENANT);
  nJobs++; JobClass::vTraceJob(CN_JOB_EXPORT);
  nJobs++; JobClass::vTraceJob(CN_JOB_INSERTROW);
  nJobs++; JobClass::vTraceJob(CN_JOB_INSERTTABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_LOADTABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_LISTPARTITIONS);
  nJobs++; JobClass::vTraceJob(CN_JOB_PUBLISH);
  nJobs++; JobClass::vTraceJob(CN_JOB_PURGE);
  nJobs++; JobClass::vTraceJob(CN_JOB_SANITY);
  nJobs++; JobClass::vTraceJob(CN_JOB_SIZEINDEX);
  nJobs++; JobClass::vTraceJob(CN_JOB_SIZETABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_SPACE);
  nJobs++; JobClass::vTraceJob(CN_JOB_STARTCLUSTER);
  nJobs++; JobClass::vTraceJob(CN_JOB_STATUS);
  nJobs++; JobClass::vTraceJob(CN_JOB_TABLESIZES);
  nJobs++; JobClass::vTraceJob(CN_JOB_TRUNCATETABLE);
  nJobs++; JobClass::vTraceJob(CN_JOB_VERIFYINDEX);
  nJobs++; JobClass::vTraceJob(CN_JOB_WAITCLUSTER);
  nJobs++; JobClass::vTraceJob(CN_JOB_WRITEBUFFER);

  // child
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBEXPORT);
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBFETCH);
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBINGESTER);
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBLOADER);
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBPUBLISH);
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBPURGER);
  nJobs++; JobClass::vTraceJob(CN_JOB_SUBTRUNCATE);
  
  // grankid
  nJobs++; JobClass::vTraceJob(CN_JOB_WRITEROW);

  //other
  nJobs++; JobClass::vTraceJob(CN_JOB_NETHOG);

  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WRITEROW - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs (%lld v. %lld)\n\n"_Stz,
           nJobs, CN_JOB_WRITEROW - CN_JOB__BEGUN + 1);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
