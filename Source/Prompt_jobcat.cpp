/*============================================================================*/
/* Prompt_jobcat.cpp -- print job catalog                                     */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_jobcat (
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
  
  // info
  nJobs++; JobClass::vJobCat(CN_JOB_NODEINFO);
  
  // boot
  nJobs++; JobClass::vJobCat(CN_JOB_BOOT);
  nJobs++; JobClass::vJobCat(CN_JOB_START);

  // system
  nJobs++; JobClass::vJobCat(CN_JOB_ALARM);
  nJobs++; JobClass::vJobCat(CN_JOB_ANSWER);
  nJobs++; JobClass::vJobCat(CN_JOB_CONNECTOR);
  nJobs++; JobClass::vJobCat(CN_JOB_PERIODIC);
  nJobs++; JobClass::vJobCat(CN_JOB_RECEIVER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUPERVISOR);
  nJobs++; JobClass::vJobCat(CN_JOB_WAKER);

  // local
  nJobs++; JobClass::vJobCat(CN_JOB_FETCHROWS);

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
  nJobs++; JobClass::vJobCat(CN_JOB_CREATEINDEX);
  nJobs++; JobClass::vJobCat(CN_JOB_PIPELINE);
  nJobs++; JobClass::vJobCat(CN_JOB_EXPORT);
  nJobs++; JobClass::vJobCat(CN_JOB_INSERTROW);
  nJobs++; JobClass::vJobCat(CN_JOB_INSERTTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_LOADTABLE);
  nJobs++; JobClass::vJobCat(CN_JOB_PUBLISH);
  nJobs++; JobClass::vJobCat(CN_JOB_PURGE);
  nJobs++; JobClass::vJobCat(CN_JOB_SANITY);
  nJobs++; JobClass::vJobCat(CN_JOB_STARTCLUSTER);
  nJobs++; JobClass::vJobCat(CN_JOB_WALK);

  // child
  nJobs++; JobClass::vJobCat(CN_JOB_SUBEXPORTER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBFETCH);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBINFLATE);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBINGESTER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBLOADER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBINSERTER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBPUBLISH);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBPURGER);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBSCAN);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBTRUNCATE);
  nJobs++; JobClass::vJobCat(CN_JOB_SUBWALKER);

  // grankid
  nJobs++; JobClass::vJobCat(CN_JOB_COPYBUFFER);
  nJobs++; JobClass::vJobCat(CN_JOB_KIDEXPORT);
  nJobs++; JobClass::vJobCat(CN_JOB_WRITEBUFFER);
  nJobs++; JobClass::vJobCat(CN_JOB_WRITEROW);

  // meta
  nJobs++; JobClass::vJobCat(CN_JOB_SELECT);

  //other
  nJobs++; JobClass::vJobCat(CN_JOB_UNIVERSAL);

  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_WRITEROW - CN_JOB__BEGUN + 1)
    vTrace("\nWarning: Missing Jobs (%lld v. %lld)\n\n"_Stz,
           nJobs, CN_JOB_WRITEROW - CN_JOB__BEGUN + 1);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
