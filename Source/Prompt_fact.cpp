/*============================================================================*/
/* Prompt_fact.cpp -- fact prompt function                                    */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_fact (
  PF_FACT,            // prompt function
  "Prompt_fact"_Stz,  // name of the prompt function
  "comment"_Stz,      // comment about the function
  "fact"_Stz,         // the prompt command
  nullptr,            // the second word
  bPrompt_fact,       // prompt function
  true,               // enabled

  true,               // visible
  "fact"_Stz,         // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_fact(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pInpTknVec();
  
  vTrace("... Fact ...\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
