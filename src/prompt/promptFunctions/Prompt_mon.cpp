/*============================================================================*/
/* Prompt_mon.hpp -- Print Monitor Information                                */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_mon (
  PF_MON,                 // prompt function
  "Prompt_mon"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "mon"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_mon,            // prompt function
  true,                   // enabled

  true,                   // visible
  "mon"_Stz,              // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_mon(SePrompt* pPrompt)
{
  s48K sPortrait;
  
  AM::vMonitorPortrait(sPortrait);
  
  vTrace("%s\n"_Stz,sPortrait.szStz());
  
  AM::vLogText(-1,sPortrait);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
