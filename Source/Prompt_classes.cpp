/*============================================================================*/
/* Prompt_classes.cpp -- classes report                                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_classes (
  "Prompt_classes"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "classes"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_threads,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern Arguments   gxArguments;
extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_classes(SePrompt* pPrompt)
{  
  vTrace("  not implemented yet\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
