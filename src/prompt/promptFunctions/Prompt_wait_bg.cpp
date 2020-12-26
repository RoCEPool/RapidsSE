/*============================================================================*/
/* Prompt_wait_bg.cpp -- wait on bg jobs                                      */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_wait_bg (
  PF_WAIT_BG,        // prompt function
  "Prompt_wait_bg"_Stz,  // name of the prompt function
  "comment"_Stz,     // comment about the function
  "wait_bg"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_wait_bg,   // prompt function
  true,              // enabled

  true,              // visible
  "wait_bg"_Stz,     // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_wait_bg(SePrompt* pPrompt)
{
  pPrompt->vNapTillBgJobsCompleted();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
