/*============================================================================*/
/* Prompt_logWrite.cpp -- Write Log to csv file                               */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_logWrite (
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
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 iNumTokens = pTokenVec->nItems();
  
  if (iNumTokens > 1 && pTokenVec->pGet(1)->eToken() == TN_NAME) {
    if (pTokenVec->pGet(1)->bSame("short"_Stz))
      Log::vWriteCsv(true);
    else
      Log::vWriteCsv(false);
  }
  else
    Log::vWriteCsv(false);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
