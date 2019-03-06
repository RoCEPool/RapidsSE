/*============================================================================*/
/* Prompt_bp.cpp -- buffer pool report                                        */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_bp (
  PF_BP,                // prompt function
  "Prompt_bp"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "bp"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_bp,           // prompt function
  true,                 // enabled

  true,                 // visible
  "bp"_Stz              // help
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
