/*============================================================================*/
/* Prompt_timerStart.cpp -- timer start                                       */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_timerStart (
  PF_NONE,                     // prompt function
  "Prompt_timerStart"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the functopm
  "timerStart"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_timerStart,      // prompt function
  true,                    // enabled

  true,                    // visible
  "fun help"_Stz           // help
);

Timer gxPromptTimer;  // the timer

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_timerStart(SePrompt* pPrompt)
{
  gxPromptTimer.vStart();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
