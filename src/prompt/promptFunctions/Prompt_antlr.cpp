/*============================================================================*/
/* Prompt_antlr.cpp -- antlr test                                             */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_antlr (
  PF_ANTLR,           // prompt function
  "Prompt_antlr"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "antlr"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_antlr,      // prompt function
  true,               // enabled

  true,               // visible
  "antlr"_Stz,        // help
  ""_Stz              // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_antlr(SePrompt* pPrompt)
{
  // Read in and parse the plan graph definitions

  PlanDefinitionParser* pPlanDefinitionParser = new PlanDefinitionParser();

  PlanNode* root = pPlanDefinitionParser->pParse();

  delete pPlanDefinitionParser;

  if (root == nullptr) {
    return false;
  }

  delete root;

//  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
//
//  s48K sAntlr;
//
//  pInpTknVec->vVecToString(sAntlr,1,true);
//
//  vTrace("antlr: %s\n"_Stz,sAntlr.azChar());
//
//  // antlr test here
//  SqlPlanner* pPlanner = new SqlPlanner();
//
//  pPlanner->plan(sAntlr.szStz());
//
//  delete pPlanner;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
