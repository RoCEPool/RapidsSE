/*============================================================================*/
/* Prompt_ssn.hpp -- Print Session Information                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_ssn (
  "Prompt_ssn"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "ssn"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_ssn,            // prompt function
  true,                   // enabled

  true,                   // visible
  "fun help"_Stz          // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ssn(SePrompt* pPrompt)
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
