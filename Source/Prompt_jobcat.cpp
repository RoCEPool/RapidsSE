/*============================================================================*/
/* Prompt_jobcat.cpp -- print job catalog                                     */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_jobcat (
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

  // local
  nJobs++; JobClass::vJobCat(CN_JOB_LOCAL);
  
  // generic
  nJobs++; JobClass::vJobCat(CN_JOB__FIND);

  // dist2pc
  nJobs++; JobClass::vJobCat(CN_JOB_DISTRIBUTED);


  vTrace("\n"_Stz);
  
  if (nJobs != CN_JOB_START - CN_JOB_BOOT + 1)
    vTrace("\nWarning: Missing Jobs (%lld v. %lld)\n\n"_Stz,
           nJobs, CN_JOB_START - CN_JOB_BOOT + 1);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
