/*============================================================================*/
/* Prompt_activityMap.hpp -- Print ActvityMap Information                     */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_activityMap (
  PF_ACTIVITYMAP,           // prompt function
  "Prompt_activityMap"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the functopm
  "activityMap"_Stz,        // the prompt command
  nullptr,                  // the second word
  bPrompt_activityMap,      // prompt function
  true,                     // enabled

  true,                     // visible
  "activityMap"_Stz,        // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_activityMap(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 iNumTokens = pTokenVec->nItems();
  
  if (iNumTokens > 1 && pTokenVec->pGet(1)->eToken() == TN_NAME) {
    if (pTokenVec->pGet(1)->bSame("short"_Stz))
      ActivityMap::vWriteCsv(true);
    else
      ActivityMap::vWriteCsv(false);
  }
  else
    ActivityMap::vWriteCsv(false);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
