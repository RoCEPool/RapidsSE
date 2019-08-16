/*============================================================================*/
/* Prompt_rmfifo.cpp -- remove fifo                                           */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_rmfifo (
  PF_RMFIFO,             // prompt function
  "Prompt_rmfifo"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "rmfifo"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_rmfifo,        // prompt function
  true,                  // enabled

  true,                  // visible
  "rmfifo"_Stz,          // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_rmfifo(SePrompt* pPrompt)
{
  s256 sFifoPath;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("rmfifo"_Stz));
  
  sFifoPath = (*pTokenVec)[1]->szText();
  
  if (! Fifo::bRemove(sFifoPath)) {
    vTrace("Failed to remove fifo: %s\n"_Stz,sFifoPath.aChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
