/*============================================================================*/
/* Prompt_rnd.cpp -- random number helper                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_rnd (
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
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("rnd"_Stz));
  
  ThirdParty::init_genrand64(0);
  
  for (i8 i = 0; i < 128; i++) {
    vTrace("%6lld "_Stz,Random::iInteger(C_1M));
    if (0 == (i+1) % 8)
      vTrace("\n"_Stz);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
