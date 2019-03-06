/*============================================================================*/
/* Prompt_heapMem.cpp -- heap memory contents                                 */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_heapMem (
  PF_HEAPMEM,           // prompt function
  "Prompt_heapMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "heapMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_heapMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_heapMem(SePrompt* pPrompt)
{
  
  HeapMem::vReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
