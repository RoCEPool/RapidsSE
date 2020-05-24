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
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("print"_Stz));
  
  //
  if (pInpTknVec->nTokens() == 1) {
    vInfo("\n"_Stz);
    return true;

  }
  
  if (pInpTknVec->nTokens() == 2) {
    vInfo("%s\n"_Stz,pInpTknVec->pGet(1)->szChars());
    return true;
  }
  
  if (pInpTknVec->nTokens() == 3) {
    
    Stz* szSectionNm = pInpTknVec->pGet(2)->szChars();
    pPrompt->vSection(szSectionNm);
    
    vInfo("\n"_Stz);
    vInfo("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
          "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
          "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
          "/\n"_Stz);
    vInfo("§Section %lld) %s : (@line %lld)\n"_Stz,
          pPrompt->iNextSectionNo(),
          pInpTknVec->pGet(2)->szChars(),
          pInpTknVec->pGet(2)->iLineNo()
          );
    vInfo("\n"_Stz);
    return true;
  }
  
  vInfo("%s\n"_Stz,pInpTknVec->pGet(1)->szChars());
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
