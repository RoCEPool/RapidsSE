/*============================================================================*/
/* Prompt_markRes.cpp -- the mark job option                                  */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"

PromptDesc __Prompt_markRes (
  PF_MARKRES,           // prompt function
  "Prompt_markRes"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "markRes"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markRes,   // prompt function
  true,              // enabled

  true,              // visible
  "markRes"_Stz,     // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markRes(SePrompt* pPrompt)
{
  PromptHelper::vEnumFlags(AM::pMarkResource(),
                           "Res"_Stz,
                           aCN,
                           CN_ENUMS,
                           pPrompt->pTknVec());

//  Stz* aTkn1 = nullptr;
//  Stz* aTkn2 = nullptr;
//
//  TokenVec* pTknVec = pPrompt->pTknVec();
//  i8 nTokens = pTknVec->nTokens();
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  if (nTokens == 1) {
//
//    for (i8 i_Resource = RK_NONE+1; i_Resource < RK_ENUMS; i_Resource++)
//      if (gpAttachMap->m_bMarkResource[(eRK)i_Resource])
//        vTrace("%s : %s\n"_Stz,
//               aRK[i_Resource],
//               gpAttachMap->m_bMarkResource[i_Resource] ? "on"_Stz : "off"_Stz);
//
//    vTrace("MarkResAll is currently : %s\n"_Stz,
//           gpAttachMap->m_bMarkResourceAll ? "on"_Stz : "off"_Stz);
//
//    return true;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  if (nTokens == 2) {
//    aTkn1 = pTknVec->pGet(1)->szText();
//
//    if (aTkn1->bSame("?"_Stz)) {
//      for (i8 i_Resource = RK_NONE+1; i_Resource < RK_ENUMS; i_Resource++)
//           vTrace("  %s\n"_Stz,aRK[i_Resource]);
//      return true;
//    }
//
//    if (aTkn1->bSame("all"_Stz)) {
//      vTrace("MarkResAll is currently : %s\n"_Stz,
//             gpAttachMap->m_bMarkResourceAll ? "on"_Stz : "off"_Stz);
//      return true;
//    }
//
//
//    for (i8 i_Resource = RK_NONE+1; i_Resource < RK_ENUMS; i_Resource++)
//      if (aTkn1->bSame(aRK[i_Resource]))
//        vTrace("%s : %s\n"_Stz,
//               aRK[i_Resource],
//               gpAttachMap->m_bMarkResource[i_Resource] ? "on"_Stz : "off"_Stz);
//    return true;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  if (nTokens == 3) {
//    aTkn1 = pTknVec->pGet(1)->szText();
//    aTkn2 = pTknVec->pGet(2)->szText();
//
//    if (aTkn1->bSame("all"_Stz)) {
//      if (aTkn2->bIsSomeTrue())
//        for (i8 i_Resource = RK_NONE+1; i_Resource < RK_ENUMS; i_Resource++)
//          gpAttachMap->m_bMarkResource[i_Resource] = true;
//      else
//        for (i8 i_Resource = RK_NONE+1; i_Resource < RK_ENUMS; i_Resource++)
//          gpAttachMap->m_bMarkResource[i_Resource] = false;
//      return true;
//    }
//
//    if (aTkn2->bIsSomeTrue()) {
//      for (i8 i_Resource = RK_NONE+1; i_Resource < RK_ENUMS; i_Resource++)
//        if (aTkn1->bSame(aRK[i_Resource]))
//          gpAttachMap->m_bMarkResource[i_Resource] = true;
//      return true;
//    }
//
//    if (aTkn2->bIsSomeFalse()) {
//      for (i8 i_Resource = RK_NONE+1; i_Resource < RK_ENUMS; i_Resource++)
//        if (aTkn1->bSame(aRK[i_Resource]))
//          gpAttachMap->m_bMarkResource[i_Resource] = false;
//      return true;
//    }
//
//    return true;
//  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
