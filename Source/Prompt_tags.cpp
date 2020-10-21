/*============================================================================*/
/* Prompt_tags.cpp -- session tags report                                     */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_tags (
  PF_TAGS,                // prompt function
  "Prompt_tags"_Stz,      // name of the prompt function
  "comment"_Stz,          // comment about the function
  "tags"_Stz,             // the prompt command
  nullptr,                // the second word
  bPrompt_tags,           // prompt function
  true,                   // enabled

  true,                   // visible
  "tags"_Stz,             // help
  ""_Stz                  // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tags(SePrompt* pPrompt)
{

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
