/*============================================================================*/
/* Prompt_rmpipe.cpp -- remove pipe                                           */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_rmpipe (
  "Prompt_rmpipe"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the functopm
  "rmpipe"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_rmpipe,        // prompt function
  true,                  // enabled

  true,                  // visible
  "fun help"_Stz         // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_rmpipe(SePrompt* pPrompt)
{
  s256 sPipe;
  
  Pipe xPipe;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("rmpipe"_Stz));
  
  sPipe = (*pTokenVec)[1]->aText();
  
  if (! xPipe.bRemove(sPipe)) {
    vTrace("Failed to remove pipe: %s\n"_Stz,sPipe.aChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
