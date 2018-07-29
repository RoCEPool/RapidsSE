/*============================================================================*/
/* Prompt_sleep.cpp -- sleep n seconds                                        */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_sleep (
  "Prompt_sleep"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "sleep"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_sleep,      // prompt post-process function
  true                // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_sleep(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("sleep"_Stz));
  
  //if ( ! bTokenCheck(2) )
  //  return false;
  
  i8 nSeconds = (*pTokenVec)[1]->iToI8();
  
  PosixThread::vSleepSec(nSeconds);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
