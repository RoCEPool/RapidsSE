/*============================================================================*/
/* Prompt_show.cpp -- show                                                    */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_show (
  PF_SHOW,           // prompt function
  "Prompt_show"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "show"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_show,      // prompt function
  true,              // enabled

  true,              // visible
  "show"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_show(SePrompt* pPrompt)
{
  TokenVec* pTkVec = pPrompt->pInpTknVec();

  ASSERT(pTkVec->pGet(0)->bSame("show"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (pTkVec->pGet(1)->bSame("grid"_Stz)) {
    
    SP_Grid spGrid;
    Grid* pGrid = AM::pGrid();
    vTrace("  %s\n"_Stz,spGrid.sz(pGrid));
    
    vTrace("\n"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (pTkVec->pGet(1)->bSame("nodes"_Stz)) {
    
    SP_Node spNode;
    for (i8 i_Node = 0; i_Node < AM::pGrid()->nDefinedNodes(); i_Node++) {
      Node* pNode = AM::pGrid()->pGetNode(i_Node);
      vTrace("  %s\n"_Stz,spNode.sz(pNode));
    }
    
    vTrace("\n"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
