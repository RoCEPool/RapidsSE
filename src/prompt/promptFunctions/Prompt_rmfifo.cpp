/*============================================================================*/
/* Prompt_rmfifo.cpp -- remove fifo                                           */
/*============================================================================*/

#include "Prompt.h"

#include "Fifo.hpp" //?

PromptFun __Prompt_rmfifo (
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
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("rmfifo"_Stz));
  
  sFifoPath = pInpTknVec->pGet(1)->szChars();
  
  if (! Fifo::bRemove(sFifoPath)) {
    vTrace("Failed to remove fifo: %s\n"_Stz,sFifoPath.azChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
