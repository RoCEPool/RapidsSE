/*============================================================================*/
/* Prompt_bye.cpp -- bye from prompt                                          */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_bye (
  PF_NONE,                     // prompt function
  "Prompt_bye"_Stz,  // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "bye"_Stz,         // the prompt command
  nullptr,           // the second word
  bPrompt_bye,       // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
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
