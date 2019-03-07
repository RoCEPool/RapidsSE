/*============================================================================*/
/* Prompt_masks.cpp -- masks report                                           */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_masks (
  PF_MASKS,             // prompt function
  "Prompt_masks"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "masks"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_masks,        // prompt function
  true,                 // enabled

  true,                 // visible
  "masks"_Stz,          // help
  ""_Stz                // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_masks(SePrompt* pPrompt)
{
  vTrace("\n"_Stz);

  vTrace("C_SIZE_POOL_BUFFER      0x%llx\n"_Stz,C_SIZE_POOL_BUFFER);
  vTrace("C_OBJECT_TO_BUFFER_MASK 0x%llx\n"_Stz,C_OBJECT_TO_BUFFER_MASK);
  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
