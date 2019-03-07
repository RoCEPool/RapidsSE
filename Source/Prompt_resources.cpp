/*============================================================================*/
/* Prompt_resources.cpp -- print resource report                              */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_resources (
  PF_RESOURCES,          // prompt function
  "Prompt_resources"_Stz,// name of the prompt function
  "comment"_Stz,         // comment about the function
  "resources"_Stz,       // the prompt command
  nullptr,               // the second word
  bPrompt_resources,     // prompt function
  true,                  // enabled

  true,                  // visible
  "resources"_Stz,       // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_resources(SePrompt* pPrompt)
{
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
