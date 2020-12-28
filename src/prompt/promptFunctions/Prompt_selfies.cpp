/*============================================================================*/
/* Prompt_selfies.cpp -- Ascii Helper                                           */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


extern SubSysCatalog gxSubSysCatalog;
extern ToolCatalog   gxToolCatalog;
extern GizmoCatalog  gxGizmoCatalog;

PromptFun __Prompt_selfies (
  PF_SELFIES,           // prompt function
  "Prompt_selfies"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "__selfies"_Stz,      // the prompt command
  nullptr,              // the second word
  bPrompt_selfies,      // prompt function
  true,                 // enabled

  false,                // visible
  "ascii"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_selfies(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
  Stz* szCmd = pInpTknVec->pGet(0)->szChars();
  
  if (gxSubSysCatalog.bCallFn(szCmd,pInpTknVec))
    return true;
  
  if (gxToolCatalog.bCallFn(szCmd,pInpTknVec))
    return true;
  
  if (gxGizmoCatalog.bCallFn(szCmd,pInpTknVec))
    return true;
  
  vTrace("error: %s : %s\n"_Stz,aEC[EC_INVALID_COMMAND],szCmd);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
