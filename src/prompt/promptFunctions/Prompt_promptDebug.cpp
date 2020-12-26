/*============================================================================*/
/* Prompt_promptDebug.cpp -- get / set prompt debug                           */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_promptDebug (
  PF_PROMPTDEBUG,           // prompt function
  "Prompt_promptDebug"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the function
  "promptDebug"_Stz,        // the prompt command
  nullptr,                  // the second word
  bPrompt_promptDebug,      // prompt function
  true,                     // enabled

  true,                     // visible
  "promptDebug"_Stz,        // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_promptDebug(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pInpTknVec();

  ASSERT(pTknVec->pGet(0)->bSame("promptDebug"_Stz));

  if (1 == pTknVec->nItems()) {
    
    vTrace("%lld\n"_Stz,iJotDebug());
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec->pGet(1)->eToken() != TN_INTEGER)
    return false;
  
  i8 iDebug = pTknVec->pGet(1)->iToI8();
  
  vJotDebug(iDebug);
  
  pPrompt->vDebugMode(iDebug);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
