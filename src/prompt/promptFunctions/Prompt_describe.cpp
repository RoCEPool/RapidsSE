/*============================================================================*/
/* Prompt_describe.cpp -- Ascii Helper                                           */
/*============================================================================*/

#include "STORE.h"

#include "eOC.h"

extern SubSysCatalog gxSubSysCatalog;

PromptFun __Prompt_describe (
  PF_DESCRIBE,           // prompt function
  "Prompt_describe"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "- describe"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_describe,      // prompt function
  true,                  // enabled

  false,                 // visible
  "ascii"_Stz,           // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_describe(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
  gxSubSysCatalog.vList();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
