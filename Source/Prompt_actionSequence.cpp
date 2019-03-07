/*============================================================================*/
/* Prompt_actionSequence.cpp -- Print ActionSequence Information              */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_actionSequence (
  PF_ACTIONSEQUENCE,           // prompt function
  "Prompt_actionSequence"_Stz, // name of the prompt function
  "comment"_Stz,               // comment about the function
  "actionSequence"_Stz,        // the prompt command
  nullptr,                     // the second word
  bPrompt_actionSequence,      // prompt function
  true,                        // enabled

  true,                        // visible
  "actionSequence"_Stz,        // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_actionSequence(SePrompt* pPrompt)
{
  ActionSequence::vWriteCsv();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
