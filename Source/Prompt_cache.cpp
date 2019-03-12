/*============================================================================*/
/* Prompt_cache.cpp -- cache helper                                           */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_cache (
  PF_CACHE,           // prompt function
  "Prompt_cache"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "cache"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_cache,      // prompt function
  true,               // enabled

  true,               // visible
  "cache"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cache(SePrompt* pPrompt)
{
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
