/*============================================================================*/
/* Prompt_globals.cpp -- print globals                                        */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "PROMPT.h"




PromptFun __Prompt_globals (
  PF_GIVEMEM,           // prompt function
  "Prompt_globals"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "globals"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_globals,      // prompt function
  true,                 // enabled

  true,                 // visible
  "globals"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_globals(SePrompt* pPrompt)
{
  AM::vPrintGlobals();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
