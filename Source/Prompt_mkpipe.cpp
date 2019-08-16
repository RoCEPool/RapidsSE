/*============================================================================*/
/* Prompt_mkpipe.cpp -- make pipe                                             */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_mkpipe (
  PF_MKPIPE,             // prompt function
  "Prompt_mkpipe"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "mkpipe"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_mkpipe,        // prompt function
  true,                  // enabled

  true,                  // visible
  "mkpipe"_Stz,          // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_mkpipe(SePrompt* pPrompt)
{
  s256 sFifoPath;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("mkpipe"_Stz));
  
  sFifoPath = (*pTokenVec)[1]->szText();
  
  if (! Fifo::bMake(sFifoPath)) {
    vTrace("Failed to make fifo: %s\n"_Stz,sFifoPath.aChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
