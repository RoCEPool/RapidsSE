/*============================================================================*/
/* Prompt_dsv.cpp -- buffer pool report                                        */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_dsv (
  PF_BP,                // prompt function
  "Prompt_dsv"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the function
  "dsv"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_dsv,          // prompt function
  true,                 // enabled

  true,                 // visible
  "dsv"_Stz,            // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_dsv(SePrompt* pPrompt)
{
  AM::pGrid()->vDsv();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
