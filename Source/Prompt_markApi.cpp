/*============================================================================*/
/* Prompt_markApi.cpp -- the mark api option                                  */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_markApi (
  PF_MARKAPI,           // prompt function
  "Prompt_markApi"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "markApi"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markApi,   // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;
extern AttachMap*    gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markApi(SePrompt* pPrompt)
{
  i8 nTokens = pPrompt->pTknVec()->nTokens();

  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 1) {
    
    for (i8 i_Api = 0; i_Api < AC_ENUMS; i_Api++)
      if (gpAttachMap->m_bMarkApi[i_Api])
        vTrace("%s : %s\n"_Stz,
               aAC[i_Api],
               gpAttachMap->m_bMarkApi[i_Api] ? "on"_Stz : "off"_Stz);
    
    vTrace("MarkApiAll is currently : %s\n"_Stz,
           gpAttachMap->m_bMarkApiAll ? "on"_Stz : "off"_Stz);
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 2) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Api = 1; i_Api < AC_ENUMS; i_Api++)
        vTrace("  %s\n"_Stz,aAC[i_Api]);
      return true;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("MarkApiAll is currently : %s\n"_Stz,
             gpAttachMap->m_bMarkApiAll ? "on"_Stz : "off"_Stz);
      return true;
    }
    
    eAC eApiCall = gpAttachMap->eFindApi(aTkn1);
    if (AC_NONE != eApiCall)
      vTrace("%s : %s\n"_Stz,
             aAC[eApiCall],
             gpAttachMap->m_bMarkApi[eApiCall] ? "on"_Stz : "off"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 3) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
    aTkn2 = pPrompt->pTknVec()->pGet(2)->aText();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        gpAttachMap->m_bMarkApiAll = true;
      else
      if (aTkn2->bIsSomeFalse())
        gpAttachMap->m_bMarkApiAll = false;
      return true;
    }

    eAC eApiCall = gpAttachMap->eFindApi(aTkn1);
    if (AC_NONE != eApiCall) {
      if (aTkn2->bIsSomeTrue())
        gpAttachMap->m_bMarkApi[eApiCall] = true;
      else
      if (aTkn2->bIsSomeFalse())
        gpAttachMap->m_bMarkApi[eApiCall] = false;
    }
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
