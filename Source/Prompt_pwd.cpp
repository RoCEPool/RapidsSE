/*============================================================================*/
/* Prompt_pwd.cpp -- print working directory                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_pwd (
  "Prompt_pwd"_Stz,   // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "pwd"_Stz,          // the prompt command
  nullptr,            // the second word
  bPrompt_pwd,        // prompt post-process function
  true                // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_pwd(SePrompt* pPrompt)
{
  Dir xDir;
  s256 sDir;
  
  TokenVec* pTokenVec = pPrompt->m_pCommandProcessor->pTknVec();
  
  ASSERT((*pTokenVec)[0]->bSame("pwd"_Stz));
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s\n"_Stz, sDir.aChar());
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
