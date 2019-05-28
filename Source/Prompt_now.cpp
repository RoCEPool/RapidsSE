/*============================================================================*/
/* Prompt_now.cpp -- show current local date and time                        */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_now (
  PF_NOW,           // prompt function
  "Prompt_now"_Stz,  // name of the prompt function
  "comment"_Stz,     // comment about the function
  "now"_Stz,         // the prompt command
  nullptr,           // the second word
  bPrompt_now,       // prompt function
  true,              // enabled

  true,              // visible
  "date"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_now(SePrompt* pPrompt)
{
  spNOW spNow;

  vTrace("%s (local)\n"_Stz,spNow.sz());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
