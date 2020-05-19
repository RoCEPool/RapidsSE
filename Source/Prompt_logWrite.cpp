/*============================================================================*/
/* Prompt_logWrite.cpp -- Write Log to csv file                               */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_logWrite (
  PF_LOGWRITE,              // prompt function
  "Prompt_logWrite"_Stz,    // name of the prompt function
  "comment"_Stz,            // comment about the function
  "logWrite"_Stz,           // the prompt command
  nullptr,                  // the second word
  bPrompt_logWrite,         // prompt function
  true,                     // enabled

  true,                     // visible
  "logWrite"_Stz,           // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_logWrite(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (iNumTokens > 1 && pInpTknVec->pGet(1)->eToken() == TN_NAME) {
    if (pInpTknVec->pGet(1)->bSame("short"_Stz))
      AM::vLogWriteCsv(true);
    else
      AM::vLogWriteCsv(false);
  }
  else
    AM::vLogWriteCsv(false);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
