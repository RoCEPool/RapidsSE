/*============================================================================*/
/* Prompt_memory.cpp -- memory statistics                                     */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_memory (
  PF_MEMORY,           // prompt function
  "Prompt_memory"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "memory"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_memory,      // prompt function
  true,                // enabled

  true,                // visible
  "memory"_Stz,        // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_memory(SePrompt* pPrompt)
{
  bool bReset = false;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  if (3 == iNumTokens)
    bReset = pInpTknVec->pGet(2)->bSame("reset"_Stz);

  PromptHelper::vMemoryStatistics();
  
  if (bReset)
    MemLib::vReset();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
