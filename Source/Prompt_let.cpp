/*============================================================================*/
/* Prompt_let.cpp -- let                                                      */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_let (
  PF_LET,           // prompt function
  "Prompt_let"_Stz, // name of the prompt function
  "comment"_Stz,    // comment about the function
  "let"_Stz,        // the prompt command
  nullptr,          // the second word
  bPrompt_let,      // prompt function
  true,             // enabled

  true,             // visible
  "let"_Stz,        // help
  ""_Stz            // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_let(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pTknVec();

  ASSERT((*pTknVec)[0]->bSame("let"_Stz));


  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
