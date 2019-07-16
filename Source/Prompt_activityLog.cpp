/*============================================================================*/
/* Prompt_activityLog.hpp -- Print ActivityLog Information                    */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_activityMap (
  PF_ACTIVITYLOG,           // prompt function
  "Prompt_activityLog"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the function
  "activityLog"_Stz,        // the prompt command
  nullptr,                  // the second word
  bPrompt_activityLog,      // prompt function
  true,                     // enabled

  true,                     // visible
  "activityLog"_Stz,        // help
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
      ActivityLog::vWriteCsv(true);
    else
      ActivityLog::vWriteCsv(false);
  }
  else
    ActivityLog::vWriteCsv(false);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
