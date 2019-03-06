/*============================================================================*/
/* Prompt_attachMap.hpp -- Print AttachMap Information                        */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_attachMap (
  PF_NONE,                     // prompt function
  "Prompt_attachMap"_Stz, // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "attachMap"_Stz,        // the prompt command
  nullptr,                // the second word
  bPrompt_attachMap,      // prompt function
  true,                   // enabled

  true,                   // visible
  "fun help"_Stz          // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_attachMap(SePrompt* pPrompt)
{
  if (gpAttachMap)
    gpAttachMap->vPrintAttachMap();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
