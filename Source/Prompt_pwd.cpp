/*============================================================================*/
/* Prompt_pwd.cpp -- print working directory                                  */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_pwd (
  PF_PWD,             // prompt function
  "Prompt_pwd"_Stz,   // name of the prompt function
  "comment"_Stz,      // comment about the function
  "pwd"_Stz,          // the prompt command
  nullptr,            // the second word
  bPrompt_pwd,        // prompt function
  true,               // enabled

  true,               // visible
  "pwd"_Stz,          // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_pwd(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("pwd"_Stz));
  
  s256 sDir;
  if (Dir::bGetCurrent(sDir))
    vTrace("%s\n"_Stz, sDir.azChar());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
