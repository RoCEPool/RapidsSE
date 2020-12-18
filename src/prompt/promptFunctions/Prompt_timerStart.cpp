/*============================================================================*/
/* Prompt_timerStart.cpp -- timer start                                       */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_timerStart (
  PF_TIMERSTART,           // prompt function
  "Prompt_timerStart"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the function
  "timerStart"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_timerStart,      // prompt function
  true,                    // enabled

  true,                    // visible
  "timerStart"_Stz,        // help
  ""_Stz                   // description
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
