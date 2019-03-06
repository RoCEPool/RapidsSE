/*============================================================================*/
/* Prompt_cmds.cpp -- prompt cmds function                                    */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_cmds (
  "Prompt_cmds"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "cmds"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_cmds,          // prompt function
  true,                  // enabled

  true,                  // visible
  "fun help"_Stz         // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cmds(SePrompt* pPrompt)
{
  

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
