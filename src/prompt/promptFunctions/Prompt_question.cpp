/*============================================================================*/
/* Prompt_quesiton.cpp -- print prompt help                                   */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "PROMPT.h"



PromptFun __Prompt_question (
  PF_QUESTION,           // prompt function
  "Prompt_question"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "?"_Stz,               // the prompt command
  nullptr,               // the second word
  bPrompt_question,      // prompt function
  true,                  // enabled

  true,                  // visible
  "?"_Stz,               // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_question(SePrompt* pPrompt)
{
  PROMPT::bPrompt_help(pPrompt);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
