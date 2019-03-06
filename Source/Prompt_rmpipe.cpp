/*============================================================================*/
/* Prompt_rmpipe.cpp -- remove pipe                                           */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_rmpipe (
  PF_RMPIPE,             // prompt function
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
  s256 sPipePath;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("rmpipe"_Stz));
  
  sPipePath = (*pTokenVec)[1]->aText();
  
  if (! Pipe::bRemove(sPipePath)) {
    vTrace("Failed to remove pipe: %s\n"_Stz,sPipePath.aChar());
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
