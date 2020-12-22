/*============================================================================*/
/* Prompt_bpa.cpp -- buffer pool analysis                                     */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_bpa (
  PF_BPA,               // prompt function
  "Prompt_bpa"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the function
  "bpa"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_bpa,          // prompt function
  true,                 // enabled

  true,                 // visible
  "bpa"_Stz,            // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_bpa(SePrompt* pPrompt)
{
  
  PoolHelper::vPoolAnalyze();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
