/*============================================================================*/
/* Prompt_pow2.cpp -- power of 2                                             */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_pow2 (
  PF_POW2,           // prompt function
  "Prompt_pow2"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "pow2"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_pow2,      // prompt function
  true,              // enabled

  true,              // visible
  "pow2"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_pow2(SePrompt* pPrompt)
{  
  u8 uVal = 1;
  
  for (i8 i = 0; i < 64; i++) {
    vTrace("%2lld: %llu\n"_Stz,i,uVal);
    uVal *= 2;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
