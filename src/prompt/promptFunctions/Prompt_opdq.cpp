/*============================================================================*/
/* Prompt_opdq.cpp -- Operand Queue Repo                                      */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_opdq (
  PF_OPDQ,              // prompt function
  "Prompt_opdq"_Stz,    // name of the prompt function
  "comment"_Stz,        // comment about the function
  "opdq"_Stz,           // the prompt command
  nullptr,              // the second word
  bPrompt_opdq,         // prompt function
  true,                 // enabled

  true,                 // visible
  "opdq"_Stz,           // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_opdq(SePrompt* pPrompt)
{
  vTrace("Operand Queue Report\n"_Stz);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();

  bool bStdOut = false;
  if (2 == iNumTokens)
    bStdOut = StrLib::bToBool(pInpTknVec->pGet(1)->azChars());
  
  
  Engine::vOperandQueueReport(bStdOut);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
