/*============================================================================*/
/* Prompt_actionSequence.cpp -- Print ActionSequence Information              */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_actionSequence (
  "Prompt_actionSequence"_Stz, // name of the prompt function
  "comment"_Stz,               // comment about the functopm
  "actionSequence"_Stz,        // the prompt command
  nullptr,                     // the second word
  bPrompt_actionSequence,      // prompt function
  true,                        // enabled

  true,                        // visible
  "fun help"_Stz               // help
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
