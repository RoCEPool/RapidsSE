/*============================================================================*/
/* Prompt_date.cpp -- show current local date and time                        */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_date (
  PF_DATE,           // prompt function
  "Prompt_date"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "date"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_date,      // prompt function
  true,              // enabled

  true,              // visible
  "date"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_date(SePrompt* pPrompt)
{
  spNOW spNow;

  vTrace("%s (local)\n"_Stz,spNow.sz());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
