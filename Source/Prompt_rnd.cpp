/*============================================================================*/
/* Prompt_rnd.cpp -- random number helper                                     */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_rnd (
  PF_RND,           // prompt function
  "Prompt_rnd"_Stz, // name of the prompt function
  "comment"_Stz,    // comment about the function
  "rnd"_Stz,        // the prompt command
  nullptr,          // the second word
  bPrompt_rnd,      // prompt function
  true,             // enabled

  true,             // visible
  "rnd"_Stz,        // help
  ""_Stz            // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_rnd(SePrompt* pPrompt)
{  
  ThirdParty::init_genrand64(0);
  
//  for (i8 i = 0; i < 1024; i++) {
//    vTrace("/* %4lld */ %20lluull, "_Stz,i,Random::uInteger());
//    if (0 == (i+1) % 1)
//      vTrace("\n"_Stz);
//  }

  i8 i_Next = 0;
  for (i8 i = 0; i < 32; i++) {
    vTrace("/* %4lld */ %20lluull, "_Stz,i,RndLib::uRepeatable(i_Next));
    if (0 == (i+1) % 1)
      vTrace("\n"_Stz);
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
