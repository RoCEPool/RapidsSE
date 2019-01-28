/*============================================================================*/
/* Prompt_mon.hpp -- Print Monitor Information                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_mon (
  "Prompt_mon"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "mon"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_mon,            // prompt function
  true,                   // enabled

  true,                   // visible
  "fun help"_Stz          // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_mon(SePrompt* pPrompt)
{
  s48K sPortrait;
  
  Engine::vMonitorPortrait(sPortrait);
  
  vTrace("%s\n"_Stz,sPortrait.aStz());
  
  ActivityMap::vMarkString(-1,sPortrait);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
