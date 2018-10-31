/*============================================================================*/
/* Prompt_markJob.cpp -- the mark job option                                  */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_markJob (
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

  if (nTokens == 1)
    return true;
  
  if (nTokens == 2) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
    pJobClass = JobClass::pFind(aTkn1, nullptr);
    if (pJobClass)
      vTrace("%s : %s\n"_Stz,
             aCN[pJobClass->m_eClass],
             pPrompt->bMarkJob(pJobClass->m_eClass) ? "on"_Stz : "off"_Stz);
    return true;
  }
  
  if (nTokens == 3) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
    aTkn2 = pPrompt->pTknVec()->pGet(2)->aText();
    
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
