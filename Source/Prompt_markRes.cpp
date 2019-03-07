/*============================================================================*/
/* Prompt_markRes.cpp -- the mark job option                                  */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_markRes (
  PF_MARKRES,           // prompt function
  "Prompt_markRes"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "markRes"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markRes,   // prompt function
  true,              // enabled

  true,              // visible
  "markRes"_Stz,     // help
  ""_Stz             // description
);

extern AttachMap* gpAttachMap;
extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markRes(SePrompt* pPrompt)
{
  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;

  TokenVec* pTknVec = pPrompt->pTknVec();
  i8 nTokens = pTknVec->nTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 1) {
    
    for (i8 i_Resource = SR_NONE+1; i_Resource < SR_ENUMS; i_Resource++)
      if (gpAttachMap->m_bMarkResource[(eSR)i_Resource])
        vTrace("%s : %s\n"_Stz,
               aSR[i_Resource],
               gpAttachMap->m_bMarkResource[i_Resource] ? "on"_Stz : "off"_Stz);
    
    vTrace("MarkResAll is currently : %s\n"_Stz,
           gpAttachMap->m_bMarkResourceAll ? "on"_Stz : "off"_Stz);

    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 2) {
    aTkn1 = pTknVec->pGet(1)->aText();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Resource = SR_NONE+1; i_Resource < SR_ENUMS; i_Resource++)
           vTrace("  %s\n"_Stz,aSR[i_Resource]);
      return true;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("MarkResAll is currently : %s\n"_Stz,
             gpAttachMap->m_bMarkResourceAll ? "on"_Stz : "off"_Stz);
      return true;
    }
    
    
    for (i8 i_Resource = SR_NONE+1; i_Resource < SR_ENUMS; i_Resource++)
      if (aTkn1->bSame(aSR[i_Resource]))
        vTrace("%s : %s\n"_Stz,
               aSR[i_Resource],
               gpAttachMap->m_bMarkResource[i_Resource] ? "on"_Stz : "off"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nTokens == 3) {
    aTkn1 = pTknVec->pGet(1)->aText();
    aTkn2 = pTknVec->pGet(2)->aText();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        for (i8 i_Resource = SR_NONE+1; i_Resource < SR_ENUMS; i_Resource++)
          gpAttachMap->m_bMarkResource[i_Resource] = true;
      else
        for (i8 i_Resource = SR_NONE+1; i_Resource < SR_ENUMS; i_Resource++)
          gpAttachMap->m_bMarkResource[i_Resource] = false;
      return true;
    }
    
    if (aTkn2->bIsSomeTrue()) {
      for (i8 i_Resource = SR_NONE+1; i_Resource < SR_ENUMS; i_Resource++)
        if (aTkn1->bSame(aSR[i_Resource]))
          gpAttachMap->m_bMarkResource[i_Resource] = true;
      return true;
    }

    if (aTkn2->bIsSomeFalse()) {
      for (i8 i_Resource = SR_NONE+1; i_Resource < SR_ENUMS; i_Resource++)
        if (aTkn1->bSame(aSR[i_Resource]))
          gpAttachMap->m_bMarkResource[i_Resource] = false;
      return true;
    }
    
    return true;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
