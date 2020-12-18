/*============================================================================*/
/* Prompt_log.cpp -- log                                                      */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_log (
  PF_LOG,             // prompt function
  "Prompt_log"_Stz,    // name of the prompt function
  "comment"_Stz,       // comment about the function
  "log"_Stz,           // the prompt command
  nullptr,             // the second word
  bPrompt_log,         // prompt function
  true,                // enabled

  true,                // visible
  "log"_Stz,           // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_log(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  AM::vLog(pInpTknVec);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
