/*============================================================================*/
/* Prompt_setDbg.cpp -- set debug                                             */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_setDbg (
  PF_SETDBG,           // prompt function
  "Prompt_setDbg"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "setDbg"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_setDbg,      // prompt function
  true,                // enabled

  true,               // visible
  "setDbg"_Stz,       // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_setDbg(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("setDbg"_Stz));
  

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
