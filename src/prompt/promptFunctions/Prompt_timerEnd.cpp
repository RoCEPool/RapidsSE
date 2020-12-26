/*============================================================================*/
/* Prompt_timerEnd.cpp -- timer end                                           */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_timerEnd (
  PF_TIMEREND,           // prompt function
  "Prompt_timerEnd"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "timerEnd"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_timerEnd,      // prompt function
  true,                  // enabled

  true,                  // visible
  "timerEnd"_Stz,        // help
  ""_Stz                 // description
);

extern Timer gxPromptTimer;  // the timer

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_timerEnd(SePrompt* pPrompt)
{
  gxPromptTimer.vUpdate();
  
  vTrace("duration %lldms\n"_Stz,gxPromptTimer.iMilliseconds());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
