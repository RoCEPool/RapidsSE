/*============================================================================*/
/* Prompt_mri.cpp -- mri                                                      */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_mri (
  PF_MEMORY,           // prompt function
  "Prompt_mri"_Stz,    // name of the prompt function
  "comment"_Stz,       // comment about the function
  "mri"_Stz,           // the prompt command
  nullptr,             // the second word
  bPrompt_mri,         // prompt function
  true,                // enabled

  true,                // visible
  "mri"_Stz,           // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_mri(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 iNumTokens = pTokenVec->nItems();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
