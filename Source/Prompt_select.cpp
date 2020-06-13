/*============================================================================*/
/* Prompt_select.cpp -- select prompt function                                */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_select (
  PF_SHELL,            // prompt function
  "Prompt_select"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "select"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_select,      // prompt function
  true,                // enabled

  true,                // visible
  "select"_Stz,        // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_select(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pInpTknVec();
  
  s48K sSelect;
  
  pTknVec->vToTknString(sSelect);
  vTrace("Select:: %s\n"_Stz,sSelect.szStz());
  
  
  Stmt_Select* pSelectStmt = new Stmt_Select;
  
  bool b = pSelectStmt->bParse(pTknVec);
  
  delete pSelectStmt;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
