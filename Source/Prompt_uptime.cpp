/*============================================================================*/
/* Prompt_uptime.cpp -- uptime prompt function                                */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_uptime (
  PF_UPTIME,             // prompt function
  "Prompt_uptime"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "uptime"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_uptime,        // prompt function
  true,                  // enabled

  true,                  // visible
  "uptime"_Stz,          // help
  ""_Stz                 // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_uptime(SePrompt* pPrompt)
{
  i8 iUpTimeSsec = AM::iElapsedSsec();

  vTrace("  %llds\n"_Stz,iUpTimeSsec);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
