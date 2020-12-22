/*============================================================================*/
/* Prompt_antlr.cpp -- antlr test                                             */
/*============================================================================*/

#include "StorageEngine.h"

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
  // TODO: This should be done once at system startup
  // Read in and parse the plan graph definitions
  PlanDefinitionParser* pPlanDefinitionParser = new PlanDefinitionParser();
  PlanNode* pRoot = pPlanDefinitionParser->pParse();
  delete pPlanDefinitionParser;

  if (pRoot == nullptr) {
    return false;
  }

  delete pRoot;

  // Parse the SQL command.

  // First get the command from the prompt
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  s48K sAntlr;
  pInpTknVec->vVecToString(sAntlr,0,true);

  vTrace("%s\n"_Stz,sAntlr.azChar());


  SqlPlanner* pPlanner = new SqlPlanner();
  pPlanner->plan(sAntlr.szStz());
  delete pPlanner;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
