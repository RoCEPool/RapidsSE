/*============================================================================*/
/* Prompt_mkfifo.cpp -- make fifo                                             */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_mkfifo (
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
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("mkfifo"_Stz));
  
  sFifoPath = (*pTokenVec)[1]->szText();
  
  if (! Fifo::bMake(sFifoPath)) {
    vTrace("Failed to make fifo: %s\n"_Stz,sFifoPath.azChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
