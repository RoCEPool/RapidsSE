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
  Token* pArg = nullptr;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 nTokens = pTokenVec->nItems();

  if (nTokens == 2)
    pArg = pTokenVec->pGet(1);
    
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
