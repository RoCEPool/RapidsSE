/*============================================================================*/
/* Prompt_timerEnd.cpp -- timer end                                           */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_timerEnd (
  "Prompt_timerEnd"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the functopm
  "timerEnd"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_timerEnd,      // prompt post-process function
  true                   // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_timerEnd(SePrompt* pPrompt)
{
  Timer* pTimer = pPrompt->pTimer();
  
  pTimer->vUpdate();
  
  vTrace("duration %lldms\n"_Stz,pTimer->iMilliseconds());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
