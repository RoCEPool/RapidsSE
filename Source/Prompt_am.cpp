/*============================================================================*/
/* Prompt_am.cpp -- write a mark into the activity map                        */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_am (
  PF_NONE,                     // prompt function
  "Prompt_am"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "am"_Stz,          // the prompt command
  nullptr,           // the second word
  bPrompt_am,        // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_am(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("am"_Stz));
  
  if (pTokenVec->nTokens() > 1)
    ActivityMap::vMarkInfo(pTokenVec->pGet(1)->aText());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
