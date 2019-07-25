/*============================================================================*/
/* Prompt_activityLog.hpp -- Print Log Information                            */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_ActivityLog (
  PF_ACTIVITYLOG,           // prompt function
  "Prompt_activityLog"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the function
  "logWrite"_Stz,           // the prompt command
  nullptr,                  // the second word
  bPrompt_activityLog,      // prompt function
  true,                     // enabled

  true,                     // visible
  "logWrite"_Stz,           // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_activityLog(SePrompt* pPrompt)
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
