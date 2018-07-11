/*============================================================================*/
/* Prompt_pwf.cpp -- print working folder                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_pwf (
  "Prompt_pwf"_Stz,   // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "pwf"_Stz,          // the prompt command
  nullptr,            // the second word
  vPrompt_pwf         // prompt post-process function
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
void Prompt::vPrompt_pwf(SePrompt* pPrompt)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
