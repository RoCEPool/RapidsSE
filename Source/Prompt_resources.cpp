/*============================================================================*/
/* Prompt_resources.cpp -- print resource report                              */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_resources (
  "Prompt_resources"_Stz,// name of the prompt function
  "comment"_Stz,         // comment about the functopm
  "resources"_Stz,       // the prompt command
  nullptr,               // the second word
  bPrompt_resources,     // prompt function
  true,                  // enabled

  true,                  // visible
  "fun help"_Stz         // help
);

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_resources(SePrompt* pPrompt)
{
  gxResourceTracker.vReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
