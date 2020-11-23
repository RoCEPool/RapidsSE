/*============================================================================*/
/* Prompt_bp.cpp -- buffer pool report                                        */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_bp (
  PF_BP,                // prompt function
  "Prompt_bp"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the function
  "bp"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_bp,           // prompt function
  true,                 // enabled

  true,                 // visible
  "bp"_Stz,             // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_bp(SePrompt* pPrompt)
{
  
  BufferPool::vReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
