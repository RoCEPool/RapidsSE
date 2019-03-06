/*============================================================================*/
/* Prompt_executing.cpp -- executing queue report                             */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_executing (
  PF_EXECUTING,           // prompt function
  "Prompt_executing"_Stz, // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "executing"_Stz,        // the prompt command
  nullptr,                // the second word
  bPrompt_executing,      // prompt function
  true,                   // enabled

  true,                   // visible
  "executing"_Stz,         // help
                                  ""_Stz                       // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_executing(SePrompt* pPrompt)
{
  gpAttachMap->m_pQueues->vDumpExecutingSet();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
