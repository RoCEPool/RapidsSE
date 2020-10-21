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
  TokenVec* pTknVec   = pPrompt->pInpTknVec();
  i8 nTokens = pTknVec->nTokens();
  
  TagSet* pPmtTagSet = pPrompt->pPmtTagSet();

  Token* pTkn;
  i8 i_Tkn = 0;

  s32  sName;
  s32  sTagNm;
  s256 sTagValue;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // tag <promptTag> = "Boo!" ;
  
  TKNpf(); // pTkn(0)
  TKNpf(); // pTkn(1)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bDelete = false;
  
  if (pTkn->bSame("delete"_Stz)) {
    bDelete = true;
    TKNpf();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bPmtTagSet = false;
  
  if (pTkn->bSame("prompt"_Stz)) {
    bPmtTagSet = true;
    TKNpf();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTkn->bSame("<"_Stz)) {
    TKNpf();
    sName = pTkn->szChars();
    TKNpf();
  }
  else {
    sName = pTkn->szChars();
  }
  
  sTagNm.vSprint("<%s>"_Stz,sName.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDelete) {
    if (bPmtTagSet)
      pPmtTagSet->vDelete(sTagNm);

    vTrace("success: 0 rows affected\n"_Stz);
    return true;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TKNpf();
  TKNpf();

  sTagValue = pTkn->szChars();

  if (bPmtTagSet)
    pPmtTagSet->vReplace(sTagNm,sTagValue);
  vTrace("success: 0 rows affected\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
