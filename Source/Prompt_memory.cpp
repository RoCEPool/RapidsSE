/*============================================================================*/
/* Prompt_memory.cpp -- memory dump                                           */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_memory (
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
  TokenVec* pTokenVec = pPrompt->pTknVec();

  i8 iNumTokens = pTokenVec->nItems();
  if (2 != iNumTokens)
    return false;
  
  //if (pTokenVec->pGet(1)->eToken() != TN_INTEGER)
  //  return false;
    
  void* aAddress = reinterpret_cast<void*>(pTokenVec->pGet(1)->iToI8());
  
  MemLib::vHexDump(aAddress,C_1KB);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
