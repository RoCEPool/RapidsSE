/*============================================================================*/
/* Prompt_hexdump.cpp -- memory hex dump                                      */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_hexdump (
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
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 iNumTokens = pInpTknVec->nItems();
  if (2 != iNumTokens)
    return false;
  
  //if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
  //  return false;
    
  void* aAddress = reinterpret_cast<void*>(pInpTknVec->pGet(1)->iToI8());
  
  MemLib::vHexDump(aAddress,C_1KB);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
