/*============================================================================*/
/* Prompt_bye.cpp -- bye from prompt                                          */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_bye (
  PF_BYE,            // prompt function
  "Prompt_bye"_Stz,  // name of the prompt function
  "comment"_Stz,     // comment about the function
  "bye"_Stz,         // the prompt command
  nullptr,           // the second word
  bPrompt_bye,       // prompt function
  true,              // enabled

  true,              // visible
  "bye"_Stz,         // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_bye(SePrompt* pPrompt)
{
  pPrompt->vContinue(false);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
