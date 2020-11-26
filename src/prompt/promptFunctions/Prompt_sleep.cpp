/*============================================================================*/
/* Prompt_sleep.cpp -- sleep n seconds                                        */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_sleep (
  PF_SLEEP,           // prompt function
  "Prompt_sleep"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "sleep"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_sleep,      // prompt function
  true,               // enabled

  true,               // visible
  "sleep"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_sleep(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("sleep"_Stz));
  
  i8 nSeconds = pInpTknVec->pGet(1)->iToI8();
  
  AM::vNapMsec(nSeconds * C_1K,CN_SEPROMPT);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
