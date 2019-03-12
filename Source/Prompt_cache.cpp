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
  CacheLine xCacheLine;
  
  MemLib::vSet(&xCacheLine, 0xFF, sizeof(xCacheLine), CN_NONE);
  
  w8 wVal = 1;
  
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE; i_Offset++) {
    xCacheLine.v1B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
