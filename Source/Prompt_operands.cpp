/*============================================================================*/
/* Prompt_operands.cpp -- show / change max operands arg                      */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_operands (
  PF_OPERANDS,       // prompt function
  "Prompt_operands"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "operands"_Stz,    // the prompt command
  nullptr,           // the second word
  bPrompt_operands,  // prompt function
  true,              // enabled

  true,              // visible
  "operands"_Stz,    // help
  ""_Stz             // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_operands(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 iNumTokens = pInpTknVec->nItems();

  if (1 == iNumTokens) {
    vTrace("Operands %lld\n"_Stz,AM::nOperandsArg());
  }
  else {
    if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
      return false;
    
    i8 nOperands = pInpTknVec->pGet(1)->iToI8();
    
    AM::vOperandsArg(nOperands);
  }
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
