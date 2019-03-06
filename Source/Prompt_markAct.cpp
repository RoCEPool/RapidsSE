/*============================================================================*/
/* Prompt_markAct.cpp -- the mark action option                               */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_markAct (
  PF_NONE,                     // prompt function
  "Prompt_markAct"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "markAct"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_markAct,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern AttachMap*    gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markAct(SePrompt* pPrompt)
{
  PromptHelper::vEnumFlags(gpAttachMap->m_oMarkAction,
                           "Act"_Stz,
                           aCN,
                           CN_ENUMS,
                           pPrompt->pTknVec());
  
//  JobClass* pJobClass = nullptr;
//  
//  Stz* aTkn1 = nullptr;
//  Stz* aTkn2 = nullptr;
//  Stz* aTkn3 = nullptr;
//
//  i8 nTokens = pPrompt->pTknVec()->nTokens();
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  if (nTokens == 1) {
//    
//    for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
//      if (gpAttachMap->m_oMarkAction.bClass((eCN)i_Class))
//        vTrace("%s : %s\n"_Stz,
//               aCN[i_Class],
//               "on"_Stz);
//    
//    vTrace("MarkActAll is currently : %s\n"_Stz,
//           gpAttachMap->m_oMarkAction.bAll() ? "on"_Stz : "off"_Stz);
//
//    return true;
//  }
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  if (nTokens == 2) {
//    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
//    
//    if (aTkn1->bSame("?"_Stz)) {
//      for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
//        if ( _bIsValidJob((eCN)i_Class))
//          vTrace("  %s\n"_Stz,aCN[i_Class]);
//      return true;
//    }
//    
//    if (aTkn1->bSame("all"_Stz)) {
//      vTrace("MarkActAll is currently : %s\n"_Stz,
//             gpAttachMap->m_oMarkAction.bAll() ? "on"_Stz : "off"_Stz);
//      return true;
//    }
//    
//    pJobClass = JobClass::pFind(aTkn1, nullptr);
//    if (pJobClass)
//      vTrace("%s : %s\n"_Stz,
//             aCN[pJobClass->m_eClass],
//             gpAttachMap->m_oMarkAction.bClass(pJobClass->m_eClass) ? "on"_Stz : "off"_Stz);
//    return true;
//  }
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  if (nTokens == 3) {
//    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
//    aTkn2 = pPrompt->pTknVec()->pGet(2)->aText();
//    
//    if (aTkn1->bSame("all"_Stz)) {
//      if (aTkn2->bIsSomeTrue())
//        gpAttachMap->m_oMarkAction.vAll(true);
//      else
//      if (aTkn2->bIsSomeFalse())
//        gpAttachMap->m_oMarkAction.vAll(false);
//      return true;
//    }
//    
//    if (aTkn2->bIsSomeTrue()) {
//      pJobClass = JobClass::pFind(aTkn1, nullptr);
//      if (pJobClass)
//        gpAttachMap->m_oMarkAction.vClass(pJobClass->m_eClass,true);
//      return true;
//    }
//
//    if (aTkn2->bIsSomeFalse()) {
//      pJobClass = JobClass::pFind(aTkn1, nullptr);
//      if (pJobClass)
//        gpAttachMap->m_oMarkAction.vClass(pJobClass->m_eClass,false);
//      return true;
//    }
//    
//    pJobClass = JobClass::pFind(aTkn1, aTkn2);
//    if (pJobClass)
//      vTrace("%s : %s\n"_Stz,
//             aCN[pJobClass->m_eClass],
//             gpAttachMap->m_oMarkAction.bClass(pJobClass->m_eClass) ? "on"_Stz : "off"_Stz);
//    return true;
//  }
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  if (nTokens == 4) {
//    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
//    aTkn2 = pPrompt->pTknVec()->pGet(2)->aText();
//    aTkn3 = pPrompt->pTknVec()->pGet(3)->aText();
//    
//    pJobClass = JobClass::pFind(aTkn1, aTkn2);
//    if (! pJobClass)
//      return true;
//    
//    if (aTkn3->bIsSomeTrue()) {
//      gpAttachMap->m_oMarkAction.vClass(pJobClass->m_eClass,true);
//      return true;
//    }
//    
//    if (aTkn3->bIsSomeFalse()) {
//      gpAttachMap->m_oMarkAction.vClass(pJobClass->m_eClass,false);
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
