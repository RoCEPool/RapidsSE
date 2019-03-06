/*============================================================================*/
/* Prompt_pwd.cpp -- print working directory                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_pwd (
  PF_PWD,             // prompt function
  "Prompt_pwd"_Stz,   // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "pwd"_Stz,          // the prompt command
  nullptr,            // the second word
  bPrompt_pwd,        // prompt function
  true,               // enabled

  true,               // visible
  "pwd"_Stz           // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_pwd(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();
  
  ASSERT((*pTokenVec)[0]->bSame("pwd"_Stz));
  
  s256 sDir;
  if (Dir::bGetCurrent(sDir))
    vTrace("%s\n"_Stz, sDir.aChar());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
