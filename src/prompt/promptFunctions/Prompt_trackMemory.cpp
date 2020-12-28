/*============================================================================*/
/* Prompt_trackMemory.cpp -- track memory                                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


PromptFun __Prompt_trackMemory (
  PF_TRACKMEMORY,             // prompt function
  "Prompt_trackMemory"_Stz,   // name of the prompt function
  "track memory"_Stz,         // comment about the function
  "trackMemory"_Stz,          // the prompt command
  nullptr,                    // the second word
  bPrompt_trackMemory,        // prompt function
  true,                       // enabled

  true,                       // visible
  "trackMemory"_Stz,          // help
  ""_Stz                      // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_trackMemory(SePrompt* pPrompt)
{
  pPrompt->vTrackMemory(true);

  vTrace("\nsuccess: 0 rows affected\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
