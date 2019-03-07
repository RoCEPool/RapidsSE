/*============================================================================*/
/* Prompt_apiState.hpp -- Print Api State Information                         */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_apiState (
  PF_APISTATE,            // prompt function
  "Prompt_apiState"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "apiState"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_apiState,       // prompt function
  true,                   // enabled

  true,                   // visible
  "apiState"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_apiState(SePrompt* pPrompt)
{
  Token* pArg = nullptr;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 nTokens = pTokenVec->nItems();

  if (nTokens == 2)
    pArg = pTokenVec->pGet(1);
  
  gpAttachMap->m_pApiState->vDump();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
