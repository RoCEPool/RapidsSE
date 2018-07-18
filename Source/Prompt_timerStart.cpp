/*============================================================================*/
/* Prompt_timerStart.cpp -- timer start                                       */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_timerStart (
  "Prompt_timerStart"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the functopm
  "timerStart"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_timerStart,      // prompt post-process function
  true                     // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_timerStart(SePrompt* pPrompt)
{
  Timer* pTimer = pPrompt->pTimer();
  
  pTimer->vStart();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
