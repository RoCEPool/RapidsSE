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
  Session* pSession = pPrompt->pSession();
  Input* pInput = pSession->m_pInput;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("print"_Stz));
  
  i8 nTokens = pInpTknVec->nTokens();
  
  if (pInpTknVec->nTokens() == 1) {
    vInfo("\n"_Stz);
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // section handling
  
  if (pInpTknVec->nTokens() >= 3) {
    
    if (pInpTknVec->pGet(1)->bSame("division"_Stz)) {
        
      Stz* szSectionNm = pInpTknVec->pGet(2)->szChars();

      pPrompt->vSection(szSectionNm);
      
      vInfo("\n"_Stz);
      vInfo("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
            "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
            "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/"
            "\n"_Stz);
      vInfo("§%s @line %lld Division %lld) "_Stz,
            pInput->szPath(),
            pInpTknVec->pGet(2)->iLineNo(),
            pPrompt->iNextSectionNo()
            );
      
      for (i8 i_Tkn = 2; i_Tkn < nTokens; i_Tkn++)
        vInfo("%s "_Stz,pInpTknVec->pGet(i_Tkn)->szChars());

      vInfo(":\n"_Stz);
      vInfo("\n"_Stz);
      
      return true;
    }
    
    if (pInpTknVec->pGet(1)->bSame("section"_Stz)) {
        
      Stz* szSectionNm = pInpTknVec->pGet(2)->szChars();

      pPrompt->vSection(szSectionNm);
      
      vInfo("\n"_Stz);
      vInfo("-- - -- - -- - -- - -- - -- - -- - -- - "
            "-- - -- - -- - -- - -- - -- - -- - -- - "
            "-- - -- - -- - -- - -- - -- - -- - -- - "
            "--\n"_Stz);
      vInfo("§%s @line %lld Section %lld) "_Stz,
            pInput->szPath(),
            pInpTknVec->pGet(2)->iLineNo(),
            pPrompt->iNextSectionNo()
            );
      
      for (i8 i_Tkn = 2; i_Tkn < nTokens; i_Tkn++)
        vInfo("%s "_Stz,pInpTknVec->pGet(i_Tkn)->szChars());

      vInfo(":\n"_Stz);
      vInfo("\n"_Stz);
      
      return true;
    }

  }
     
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTokens; i_Tkn++)
    vInfo("%s "_Stz,pInpTknVec->pGet(i_Tkn)->szChars());

  return true;
}
/*----------------------------------------------------------------------------*/
//bool Prompt::bPrompt_print(SePrompt* pPrompt)
//{
//  Session* pSession = pPrompt->pSession();
//  Input* pInput = pSession->m_pInput;
//
//  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
//
//  ASSERT(pInpTknVec->pGet(0)->bSame("print"_Stz));
//
//  //
//  if (pInpTknVec->nTokens() == 1) {
//    vInfo("\n"_Stz);
//    return true;
//
//  }
//
//  if (pInpTknVec->nTokens() == 2) {
//    vInfo("%s\n"_Stz,pInpTknVec->pGet(1)->szChars());
//    return true;
//  }
//
//  if (pInpTknVec->nTokens() == 3) {
//
//    Stz* szSectionNm = pInpTknVec->pGet(2)->szChars();
//    pPrompt->vSection(szSectionNm);
//
//    vInfo("\n"_Stz);
//    vInfo("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
//          "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
//          "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
//          "/\n"_Stz);
//    vInfo("§%s @line %lld Section %lld) %s : \n"_Stz,
//          pInput->szPath(),
//          pInpTknVec->pGet(2)->iLineNo(),
//          pPrompt->iNextSectionNo(),
//          pInpTknVec->pGet(2)->szChars()
//          );
//    vInfo("\n"_Stz);
//    return true;
//  }
//
//  vInfo("%s\n"_Stz,pInpTknVec->pGet(1)->szChars());
//  return true;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
