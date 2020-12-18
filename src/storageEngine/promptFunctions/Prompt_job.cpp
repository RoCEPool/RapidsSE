/*============================================================================*/
/* Prompt_job.hpp -- Print Job Information                                    */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_job (
  PF_JOB,                 // prompt function
  "Prompt_job"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "job"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_job,            // prompt function
  true,                   // enabled

  true,                   // visible
  "job"_Stz,              // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_job(SePrompt* pPrompt)
{
  Token* pArg0 = nullptr;
  Token* pArg1 = nullptr;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pInpTknVec->nItems();
  
  if (nTokens != 3)
    return false;
  
  pArg0 = pInpTknVec->pGet(1);
  pArg1 = pInpTknVec->pGet(2);
  
  s32 sNumber;
  sNumber.vConcat(pArg1->szChars());
  vTrace("Job: %s\n"_Stz,sNumber.szStz());
  
  i8 iNumber = sNumber.iToI8();
  
  Job* pJob = _pJob(iNumber);
  
  if (! pJob) {
    vTrace("Not a job\n"_Stz);
    return false;
  }
  
  pJob->vReport();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
