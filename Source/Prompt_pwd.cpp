/*============================================================================*/
/* Prompt_pwd.cpp -- print working directory                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_pwd (
  "Prompt_pwd"_Stz,   // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "pwd"_Stz,          // the prompt command
  nullptr,            // the second word
  vPrompt_pwd         // prompt post-process function
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
void Prompt::vPrompt_pwd(SePrompt* pPrompt)
{
  Dir xDir;
  s256 sDir;
  
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("pwd"_Stz));
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s"_Stz, sDir.aChar());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
