/*============================================================================*/
/* Prompt_hexdump.cpp -- memory hex dump                                      */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_hexdump (
  PF_HEXDUMP,           // prompt function
  "Prompt_hexdump"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "hexdump"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_hexdump,      // prompt function
  true,                 // enabled

  true,                 // visible
  "hexdump"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_hexdump(SePrompt* pPrompt)
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
