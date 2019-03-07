/*============================================================================*/
/* Prompt_prefixes.cpp -- bye from prompt                                          */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_prefixes (
  PF_PREFIXES,            // prompt function
  "Prompt_prefixes"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "prefixes"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_prefixes,       // prompt function
  true,                   // enabled

  true,                   // visible
  "prefixes"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_prefixes(SePrompt* pPrompt)
{
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
