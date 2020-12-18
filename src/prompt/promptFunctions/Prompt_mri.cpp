/*============================================================================*/
/* Prompt_mri.cpp -- mri                                                      */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_mri (
  PF_MRI,              // prompt function
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
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  AM::vMri(pInpTknVec);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
