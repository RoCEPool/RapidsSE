/*============================================================================*/
/* Prompt_mkfifo.cpp -- make fifo                                             */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_mkfifo (
  PF_MKFIFO,             // prompt function
  "Prompt_mkfifo"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "mkfifo"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_mkfifo,        // prompt function
  true,                  // enabled

  true,                  // visible
  "mkfifo"_Stz,          // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_mkfifo(SePrompt* pPrompt)
{
  s256 sFifoPath;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("mkfifo"_Stz));
  
  sFifoPath = pInpTknVec->pGet(1)->szChars();
  
  if (! Fifo::bMake(sFifoPath)) {
    vTrace("Failed to make fifo: %s\n"_Stz,sFifoPath.azChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
