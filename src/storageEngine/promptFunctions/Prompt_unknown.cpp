/*============================================================================*/
/* Prompt_unknown.cpp -- Ascii Helper                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern SubSysCatalog gxSubSysCatalog;

PromptFun __Prompt_unknown (
  PF_UNKNOWN,           // prompt function
  "Prompt_unknown"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "__unknown"_Stz,      // the prompt command
  nullptr,              // the second word
  bPrompt_unknown,      // prompt function
  true,                 // enabled

  false,                // visible
  "ascii"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_unknown(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
  Stz* szSubSysNm = pInpTknVec->pGet(0)->szChars();
  
  if (gxSubSysCatalog.bCallFn(szSubSysNm,pInpTknVec))
    return true;
  
  vTrace("error: %s\n"_Stz,aEC[EC_INVALID_COMMAND]);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
