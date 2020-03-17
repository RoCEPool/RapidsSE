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
  vTrace("Tkn[%lld] %s\n"_Stz,i_Tkn,pTkn->szTerm()); \
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
  
  i8 i_Tkn = 0;
  Token* pTkn;

  s32  sTagNm;
  s256 sTagValue;
  
  TKNpf();
  TKNpf();
  
  if (pTkn->bSame("<"_Stz)) {
    TKNpf();
    TKNpf();
  }

  TKNpf();
  TKNpf();

  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
