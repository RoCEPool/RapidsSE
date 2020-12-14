/*============================================================================*/
/* Prompt_cd.cpp -- change working directory                                  */
/*============================================================================*/

#include "Prompt.h"

#include "Dir.hpp" //?

PromptFun __Prompt_cd (
  PF_CD,             // prompt function
  "Prompt_cd"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the function
  "cd"_Stz,          // the prompt command
  nullptr,           // the second word
  bPrompt_cd,        // prompt function
  true,              // enabled

  true,              // visible
  "cd"_Stz,          // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cd(SePrompt* pPrompt)
{
  s256 sDir;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("cd"_Stz));
  
  sDir = pInpTknVec->pGet(1)->szChars();
  
  i8 i_Token;
  for (i_Token = 2; i_Token < C_MAX_TOKENS; i_Token++) {
    if (! pInpTknVec->pGet(i_Token)->bHasChars())
      break;
    sDir.vConcat(pInpTknVec->pGet(i_Token)->szChars());
  }
  
  if ( ! Dir::bChangeCurrent(sDir) ) {
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
