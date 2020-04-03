/*============================================================================*/
/* Prompt_tag.cpp -- session tag managememnt                                  */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_tag (
  PF_TAG,                 // prompt function
  "Prompt_tag"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "tag"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_tag,            // prompt function
  true,                   // enabled

  true,                   // visible
  "tag"_Stz,              // help
  ""_Stz                  // description
);

#define TKNpf() { \
  if (i_Tkn >= nTokens) { \
    vTrace("Invalid tag command\n"_Stz); \
    return false; \
  } \
  pTkn = pTknVec->pGet(i_Tkn); \
  i_Tkn++; \
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tag(SePrompt* pPrompt)
{
  TokenVec* pTknVec   = pPrompt->pTokenVec();
  i8 nTokens = pTknVec->nTokens();

  Session*  pSession  = pPrompt->pSession();
  
  Semantic* pSemantic = pSession->m_pSemantic;
  EnameSet* pEnameSet = pSemantic->pEnameSet();
  
  Token* pTkn;
  i8 i_Tkn = 0;

  s32  sName;
  s32  sTagNm;
  s256 sTagValue;

  TKNpf();
  TKNpf();

  bool bDelete = false;
  
  if (pTkn->bSame("delete"_Stz)) {
    bDelete = true;
    TKNpf();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTkn->bSame("<"_Stz)) {
    TKNpf();
    sName = pTkn->szTerm();
    TKNpf();
  }
  else {
    sName = pTkn->szTerm();
  }
  
  sTagNm.vSprint("<%s>"_Stz,sName.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDelete) {
    pEnameSet->vDelete(sTagNm);
    vTrace("success: 0 rows affected\n"_Stz);
    return true;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TKNpf();
  TKNpf();

  sTagValue = pTkn->szTerm();

  if (pEnameSet->bHasName(sTagNm))
    pEnameSet->vReplace(sTagNm,sTagValue);
  else
    pEnameSet->vInsert(sTagNm,sTagValue);
  vTrace("success: 0 rows affected\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
