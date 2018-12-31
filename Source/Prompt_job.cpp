/*============================================================================*/
/* Prompt_job.hpp -- Print Job Information                                    */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_job (
  "Prompt_job"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "job"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_job,            // prompt function
  true,                   // enabled

  true,                   // visible
  "fun help"_Stz          // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_job(SePrompt* pPrompt)
{
  Token* pArg0 = nullptr;
  Token* pArg1 = nullptr;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 nTokens = pTokenVec->nItems();
  
  if (nTokens != 3)
    return false;
  
  pArg0 = pTokenVec->pGet(1);
  pArg1 = pTokenVec->pGet(2);
  
  s32 sNumber;
  sNumber.vConcat(pArg1->aText());
  vTrace("Job: %s\n"_Stz,sNumber.aStz());
  
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
