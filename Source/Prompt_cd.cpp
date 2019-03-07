/*============================================================================*/
/* Prompt_cd.cpp -- change working directory                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_cd (
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
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("cd"_Stz));
  
  sDir = (*pTokenVec)[1]->aText();
  
  i8 i_Token;
  for (i_Token = 2; i_Token < C_TOKENS; i_Token++) {
    if (! (*pTokenVec)[i_Token]->bHasText())
      break;
    sDir.vConcat((*pTokenVec)[i_Token]->aText());
  }
  
  if ( ! Dir::bChangeCurrent(sDir) ) {
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
