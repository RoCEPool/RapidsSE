/*============================================================================*/
/* Prompt_tools.cpp -- System Tools                                          */
/*============================================================================*/

#include "STORE.h"

#include "eOC.h"

PromptFun __Prompt_tools (
  PF_TOOLS,              // prompt function
  "Prompt_tools"_Stz,    // name of the prompt function
  "comment"_Stz,         // comment about the function
  "- tools"_Stz,           // the prompt command
  nullptr,               // the second word
  bPrompt_tools,         // prompt function
  true,                  // enabled

  false,                 // visible
  "tools"_Stz,           // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tools(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
