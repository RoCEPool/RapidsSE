/*============================================================================*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_print (
  PF_PRINT,          // prompt function
  "Prompt_print"_Stz,// name of the prompt function
  "comment"_Stz,     // comment about the function
  "print"_Stz,       // the prompt command
  nullptr,           // the second word
  bPrompt_print,     // prompt function
  true,              // enabled

  true,              // visible
  "print"_Stz,       // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_print(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pInpTknVec();

  ASSERT(pTokenVec->pGet(0)->bSame("print"_Stz));
  
  //
  if (pTokenVec->nTokens() == 1) {
    vInfo("\n"_Stz);
    return true;

  }
  
  if (pTokenVec->nTokens() == 2) {
    vInfo("%s\n"_Stz,pTokenVec->pGet(1)->szChars());
    return true;
  }
  
  if (pTokenVec->nTokens() == 3) {
    vInfo("\n"_Stz);
    vInfo("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
          "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
          "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
          "/\n"_Stz);
    vInfo("Section §%lld) %s:\n"_Stz,
          pPrompt->iNextSectionNo(),
          pTokenVec->pGet(2)->szChars());
    vInfo("\n"_Stz);
    return true;
  }
  
  vInfo("%s\n"_Stz,pTokenVec->pGet(1)->szChars());
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
