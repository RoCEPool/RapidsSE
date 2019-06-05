/*============================================================================*/
/* Prompt_jobcat.cpp -- print job catalog                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_jobcat (
  PF_JOBCAT,           // prompt function
  "Prompt_jobcat"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "jobcat"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_jobcat,      // prompt function
  true,                // enabled

  true,                // visible
  "jobcat"_Stz,        // help
  ""_Stz               // description
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
  nJobs++; JobClass::vJobCat(CN_JOB_BOOT);

  // info
  nJobs++; JobClass::vJobCat(CN_JOB_NODEINFO);
  
  // system
  nJobs++; JobClass::vJobCat(CN_JOB_ALARM);
  nJobs++; JobClass::vJobCat(CN_JOB_ANSWER);
  nJobs++; JobClass::vJobCat(CN_JOB_CONNECTOR);
  nJobs++; JobClass::vJobCat(CN_JOB_PERIODIC);
  nJobs++; JobClass::vJobCat(CN_JOB_RECEIVER);
  nJobs++; JobClass::vJobCat(CN_JOB_START);
  nJobs++; JobClass::vJobCat(CN_JOB_SUPERVISOR);

  // local
  nJobs++; JobClass::vJobCat(CN_JOB_CAT);
  nJobs++; JobClass::vJobCat(CN_JOB_CF);
  nJobs++; JobClass::vJobCat(CN_JOB_COPYBUFFER);
  nJobs++; JobClass::vJobCat(CN_JOB_DOCOL);
  nJobs++; JobClass::vJobCat(CN_JOB_DUMPINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_DUMPTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_FETCHROWS);
  nJobs++; JobClass::vJobCat(CN_JOB_GENERATE);
  nJobs++; JobClass::vJobCat(CN_JOB_INFLATEROWS);
  nJobs++; JobClass::vJobCat(CN_JOB_INGEST);
  nJobs++; JobClass::vJobCat(CN_JOB_LF);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTCATALOGS);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTCOLUMNS);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTIDXCOLUMNS);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTINDEXES);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTPARTCOLUMNS);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTSCHEMAS);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTTABLES);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTTENANTS);
  nJobs++; JobClass::vJobCat(CN_JOB_MONITOR);
  nJobs++; JobClass::vJobCat(CN_JOB_PING);
  nJobs++; JobClass::vJobCat(CN_JOB_PWF);
  nJobs++; JobClass::vJobCat(CN_JOB_REPEATER);
  nJobs++; JobClass::vJobCat(CN_JOB_REPORT);
  nJobs++; JobClass::vJobCat(CN_JOB_SELECTTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_SHOWCLUSTER);
  nJobs++; JobClass::vJobCat(CN_JOB_SINK);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBINSERTER);
  nJobs++; JobClass::vJobCat(CN_JOB_USESCHEMA);
  nJobs++; JobClass::vJobCat(CN_JOB_VISUALIZEINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_VISUALIZETABLE);
 
  // generic
  nJobs++; JobClass::vJobCat(CN_JOB__BEGUN);
  nJobs++; JobClass::vJobCat(CN_JOB__CANCEL);
  nJobs++; JobClass::vJobCat(CN_JOB__CANCELLED);
  nJobs++; JobClass::vJobCat(CN_JOB__END);
  nJobs++; JobClass::vJobCat(CN_JOB__ENDED);
  nJobs++; JobClass::vJobCat(CN_JOB__EXECUTE);
  nJobs++; JobClass::vJobCat(CN_JOB__EXECUTED);
  nJobs++; JobClass::vJobCat(CN_JOB__FIND);

  // dist2pc
  nJobs++; JobClass::vJobCat(CN_JOB_ANALYZEINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_ANALYZETABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_COMPACTINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_CONNECTCLUSTER);
  nJobs++; JobClass::vJobCat(CN_JOB_CREATECATALOG);
  nJobs++; JobClass::vJobCat(CN_JOB_CREATEINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_CREATESCHEMA);
  nJobs++; JobClass::vJobCat(CN_JOB_CREATETABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_CREATETENANT);
  nJobs++; JobClass::vJobCat(CN_JOB_DROPCATALOG);
  nJobs++; JobClass::vJobCat(CN_JOB_DROPINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_DROPSCHEMA);
  nJobs++; JobClass::vJobCat(CN_JOB_DROPTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_DROPTENANT);
  nJobs++; JobClass::vJobCat(CN_JOB_EXPORT);
  nJobs++; JobClass::vJobCat(CN_JOB_INSERTROW);
  nJobs++; JobClass::vJobCat(CN_JOB_INSERTTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_LOADTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_LISTPARTITIONS);
  nJobs++; JobClass::vJobCat(CN_JOB_PUBLISH);
  nJobs++; JobClass::vJobCat(CN_JOB_PURGE);
  nJobs++; JobClass::vJobCat(CN_JOB_SANITY);
  nJobs++; JobClass::vJobCat(CN_JOB_SIZEINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_SIZETABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_SPACE);
  nJobs++; JobClass::vJobCat(CN_JOB_STARTCLUSTER);
  nJobs++; JobClass::vJobCat(CN_JOB_STATUS);
  nJobs++; JobClass::vJobCat(CN_JOB_TABLESIZES);
  nJobs++; JobClass::vJobCat(CN_JOB_TRUNCATETABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_VERIFYINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_VERIFYTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_WAITCLUSTER);
  nJobs++; JobClass::vJobCat(CN_JOB_WALK);

  // child
  nJobs++; JobClass::vJobCat(CN_JOB_SUBEXPORTER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBFETCH);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBINFLATE);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBINGESTER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBLOADER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBPUBLISH);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBPURGER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBSCAN);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBTRUNCATE);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBWALKER);

  // grankid
  nJobs++; JobClass::vJobCat(CN_JOB_KIDEXPORT);
  nJobs++; JobClass::vJobCat(CN_JOB_WRITEBUFFER);
  nJobs++; JobClass::vJobCat(CN_JOB_WRITEROW);

  //other
  nJobs++; JobClass::vJobCat(CN_JOB_HEALTH);
  nJobs++; JobClass::vJobCat(CN_JOB_NETHOG);

  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WRITEROW - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs (%lld v. %lld)\n\n"_Stz,
           nJobs, CN_JOB_WRITEROW - CN_JOB__BEGUN + 1);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
