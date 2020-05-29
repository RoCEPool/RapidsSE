/*============================================================================*/
/* Prompt_markTrc.cpp -- the mark trace option                                */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"

PromptDesc __Prompt_markTrc (
  PF_MARKTRC,           // prompt function
  "Prompt_markTrc"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "markTrc"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_markTrc,      // prompt function
  true,                 // enabled

  true,                 // visible
  "markTrc"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markTrc(SePrompt* pPrompt)
{
  PromptHelper::vEnumFlags(AM::pMarkTrace(),
                           "Trc"_Stz,
                           aCN,
                           CN_ENUMS,
                           pPrompt->pTknVec());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
