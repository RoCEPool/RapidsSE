/*============================================================================*/
/* Prompt_threads.cpp -- threads report                                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_threads (
  PF_THREADS,           // prompt function
  "Prompt_threads"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "threads"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_threads,      // prompt function
  true,                 // enabled

  true,                 // visible
  "threads"_Stz,        // help
  ""_Stz                // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_threads(SePrompt* pPrompt)
{  
  Queues::vThreadsReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
