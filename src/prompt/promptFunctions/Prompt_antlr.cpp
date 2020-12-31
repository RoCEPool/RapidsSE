/*============================================================================*/
/* Prompt_antlr.cpp -- antlr test                                             */
/*============================================================================*/

#include "BASE.h"
#include "PROMPT.h"

#include "PlanNode.hpp"
#include "PlanDefinitionParser.hpp"
#include "SqlPlanner.hpp"

using namespace Parser;

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
bool PROMPT::bPrompt_antlr(SePrompt* pPrompt)
{
  // Read in and parse the plan graph definitions
  // TODO: This should be done once at system startup
  PlanDefinitionParser* pPlanDefinitionParser = new PlanDefinitionParser();
  PlanNode* pGraphTemplate = pPlanDefinitionParser->pParse();
  delete pPlanDefinitionParser;

  if (pGraphTemplate == nullptr) {
    return false;
  }


  // Parse the SQL command.

  // First get the command from the prompt
  // TODO: Ideally, get the raw command entered, not a tokenised reconstruction
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  s48K sAntlr;
  pInpTknVec->vVecToString(sAntlr,0,true);
  vTrace("%s\n"_Stz,sAntlr.azChar());

  SqlPlanner* pPlanner = new SqlPlanner();
  pPlanner->plan(sAntlr.szStz(), pGraphTemplate);
  delete pPlanner;

  // TODO: Shouldn't need to be deleted once this is done on system startup
  delete pGraphTemplate;
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
