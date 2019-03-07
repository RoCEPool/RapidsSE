/*============================================================================*/
/* Prompt_sleep.cpp -- sleep n seconds                                        */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_sleep (
  PF_SLEEP,           // prompt function
  "Prompt_sleep"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the functopm
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
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("sleep"_Stz));
  
  i8 nSeconds = (*pTokenVec)[1]->iToI8();
  
  Pthread::vSleepSec(nSeconds,CN_SEPROMPT);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
