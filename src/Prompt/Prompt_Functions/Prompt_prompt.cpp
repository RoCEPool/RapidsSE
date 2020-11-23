/*============================================================================*/
/* Prompt_prompt.cpp -- prompt settings managememnt                           */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_prompt (
  PF_PROMPT,              // prompt function
  "Prompt_prompt"_Stz,    // name of the prompt function
  "comment"_Stz,          // comment about the function
  "prompt"_Stz,           // the prompt command
  nullptr,                // the second word
  bPrompt_prompt,         // prompt function
  true,                   // enabled

  true,                   // visible
  "prompt"_Stz,           // help
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
bool Prompt::bPrompt_prompt(SePrompt* pPrompt)
{
  TokenVec* pTknVec   = pPrompt->pInpTknVec();
  i8 nTokens = pTknVec->nTokens();

  TagSet* pSettings = pPrompt->pPmtTagSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (1 == nTokens) {
    pSettings->vDumpSet();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
    sName = pTkn->szChars();
    TKNpf();
  }
  else {
    sName = pTkn->szChars();
  }
  
  sTagNm.vSprint("<%s>"_Stz,sName.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDelete) {
    pSettings->vDelete(sTagNm);
    return true;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TKNpf();
  TKNpf();

  sTagValue = pTkn->szChars();

  pSettings->vInsert(sTagNm,sTagValue);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
