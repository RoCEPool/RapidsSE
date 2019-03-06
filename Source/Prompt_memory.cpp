/*============================================================================*/
/* Prompt_memory.cpp -- memory statistics                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_memory (
  PF_NONE,                     // prompt function
  "Prompt_memory"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the functopm
  "memory"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_memory,      // prompt function
  true,                // enabled

  true,                // visible
  "fun help"_Stz       // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_memory(SePrompt* pPrompt)
{
  bool bReset = false;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 iNumTokens = pTokenVec->nItems();
  if (3 == iNumTokens)
    bReset = pTokenVec->pGet(2)->bSame("reset"_Stz);

  MemLib::vStatistics();
  
  if (bReset)
    MemLib::vReset();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
