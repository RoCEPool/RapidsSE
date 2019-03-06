/*============================================================================*/
/* Prompt_markJob.cpp -- the mark job option                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_markJob (
  PF_NONE,                     // prompt function
  "Prompt_markJob"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "markJob"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markJob,   // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markJob(SePrompt* pPrompt)
{
  JobClass* pJobClass = nullptr;
  
  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;
  Stz* aTkn3 = nullptr;

  i8 nTokens = pPrompt->pTknVec()->nTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 1) {
    
    for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
      if (pPrompt->bMarkJob((eCN)i_Class))
        vTrace("%s : %s\n"_Stz,
               aCN[i_Class],
               pPrompt->bMarkJob((eCN)i_Class) ? "on"_Stz : "off"_Stz);
    
    vTrace("MarkJobAll is currently : %s\n"_Stz,
           pPrompt->bMarkAll() ? "on"_Stz : "off"_Stz);

    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 2) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
        if ( _bIsValidJob((eCN)i_Class))
          vTrace("  %s\n"_Stz,aCN[i_Class]);
      return true;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("MarkJobAll is currently : %s\n"_Stz,
             pPrompt->bMarkAll() ? "on"_Stz : "off"_Stz);
      return true;
    }
    
    pJobClass = JobClass::pFind(aTkn1, nullptr);
    if (pJobClass)
      vTrace("%s : %s\n"_Stz,
             aCN[pJobClass->m_eClass],
             pPrompt->bMarkJob(pJobClass->m_eClass) ? "on"_Stz : "off"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 3) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
    aTkn2 = pPrompt->pTknVec()->pGet(2)->aText();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        pPrompt->vMarkAll(true);
      else
      if (aTkn2->bIsSomeFalse())
        pPrompt->vMarkAll(false);
      return true;
    }
    
    if (aTkn2->bIsSomeTrue()) {
      pJobClass = JobClass::pFind(aTkn1, nullptr);
      if (pJobClass)
        pPrompt->vMarkJob(pJobClass->m_eClass,true);
      return true;
    }

    if (aTkn2->bIsSomeFalse()) {
      pJobClass = JobClass::pFind(aTkn1, nullptr);
      if (pJobClass)
        pPrompt->vMarkJob(pJobClass->m_eClass,false);
      return true;
    }
    
    pJobClass = JobClass::pFind(aTkn1, aTkn2);
    if (pJobClass)
      vTrace("%s : %s\n"_Stz,
             aCN[pJobClass->m_eClass],
             pPrompt->bMarkJob(pJobClass->m_eClass) ? "on"_Stz : "off"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 4) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
    aTkn2 = pPrompt->pTknVec()->pGet(2)->aText();
    aTkn3 = pPrompt->pTknVec()->pGet(3)->aText();
    
    pJobClass = JobClass::pFind(aTkn1, aTkn2);
    if (! pJobClass)
      return true;
    
    if (aTkn3->bIsSomeTrue()) {
      pPrompt->vMarkJob(pJobClass->m_eClass,true);
      return true;
    }
    
    if (aTkn3->bIsSomeFalse()) {
      pPrompt->vMarkJob(pJobClass->m_eClass,false);
      return true;
    }
    
    return true;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
