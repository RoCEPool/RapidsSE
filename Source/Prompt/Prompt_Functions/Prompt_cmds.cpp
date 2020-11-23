/*============================================================================*/
/* Prompt_cmds.cpp -- prompt cmds function                                    */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_cmds (
  PF_CMDS,               // prompt function
  "Prompt_cmds"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "cmds -DISABLED-"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_cmds,          // prompt function
  true,                  // enabled

  true,                  // visible
  "cmds"_Stz,            // help
  ""_Stz                 // description
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cmds(SePrompt* pPrompt)
{
  
  RASSERT(0);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
