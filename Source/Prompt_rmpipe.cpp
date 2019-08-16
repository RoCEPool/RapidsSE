/*============================================================================*/
/* Prompt_rmpipe.cpp -- remove pipe                                           */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_rmpipe (
  PF_RMPIPE,             // prompt function
  "Prompt_rmpipe"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "rmpipe"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_rmpipe,        // prompt function
  true,                  // enabled

  true,                  // visible
  "rmpipe"_Stz,          // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_rmpipe(SePrompt* pPrompt)
{
  s256 sFifoPath;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("rmpipe"_Stz));
  
  sFifoPath = (*pTokenVec)[1]->szText();
  
  if (! Fifo::bRemove(sFifoPath)) {
    vTrace("Failed to remove pipe: %s\n"_Stz,sFifoPath.aChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
