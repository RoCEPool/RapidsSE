/*============================================================================*/
/* Prompt_calc.cpp -- calc                                                    */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_calc (
  PF_BYE,            // prompt function
  "Prompt_calc"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "calc"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_calc,      // prompt function
  true,              // enabled

  true,              // visible
  "calc"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_calc(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pInpTknVec();

  Calculator* pCalculator = new Calculator;
 
  if (! pCalculator->bCompile(pTknVec, nullptr, 1, pTknVec->nTokens() - 1)) {
    delete pCalculator;
    return true;
  }
/*
  //                     a    b    c    d    e    f    g    h    i
  i8 aiColValues[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900 };
  
  i8 iResult = 0;

  //for (i8 i = 0; i < C_10M; i++)
    iResult = pCalculator->iEvaluate(aiColValues);
*/
  
  i8 iResult = pCalculator->iCalc();
  
  vTrace("  %lld\n"_Stz,iResult);
  
  delete pCalculator;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
