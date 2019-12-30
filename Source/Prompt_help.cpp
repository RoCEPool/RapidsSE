/*============================================================================*/
/* Prompt_help.cpp -- print prompt help                                       */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_help (
  PF_HELP,           // prompt function
  "Prompt_help"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "man"_Stz,         // the prompt command
  nullptr,           // the second word
  bPrompt_help,      // prompt function
  true,              // enabled

  true,              // visible
  "man"_Stz,         // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_help(SePrompt* pPrompt)
{
  vTrace("Help\n"_Stz);
  
  SysCall oSysCall;
  
  s256 sCommand;
  
  sCommand.vSprint("man ./ManPages/HelpText"_Stz);

  IGNORE_RETURN(Shell::iExecute(sCommand,oSysCall));
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
