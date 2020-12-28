/*============================================================================*/
/* Prompt_help.cpp -- print prompt help                                       */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "PROMPT.h"



extern PromptCatalog gxPromptCatalog;

PromptFun __Prompt_help (
  PF_HELP,           // prompt function
  "Prompt_help"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "help"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_help,      // prompt function
  true,              // enabled

  true,              // visible
  "help"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_help(SePrompt* pPrompt)
{
  vTrace("Help\n"_Stz);
  
//  SysCall oSysCall;
//
//  s256 sCommand;
//
//  sCommand.vSprint("man ./ManPages/HelpText"_Stz);
//
//  IGNORE_RETURN(Shell::iExecute(sCommand,oSysCall));
  
  
  
  gxPromptCatalog.vHelp();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
