/*============================================================================*/
/* Prompt_help.cpp -- print prompt help                                       */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_help (
  PF_NONE,                     // prompt function
  "Prompt_help"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "man"_Stz,         // the prompt command
  nullptr,           // the second word
  bPrompt_help,      // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
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
