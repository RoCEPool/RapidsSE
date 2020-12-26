/*============================================================================*/
/* Prompt_heapSummary.cpp -- heap summary report                              */
/*============================================================================*/

#include "STORE.h"

PromptFun __Prompt_heapSummary (
  PF_HEAPSUMMARY,           // prompt function
  "Prompt_heapSummary"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the function
  "heapSummary"_Stz,        // the prompt command
  nullptr,                  // the second word
  bPrompt_heapSummary,      // prompt function
  true,                     // enabled

  true,                     // visible
  "heapSummary"_Stz,        // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_heapSummary(SePrompt* pPrompt)
{
  
  HeapMem::vSummaryReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
