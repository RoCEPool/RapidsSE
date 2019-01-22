/*============================================================================*/
/* Prompt_activityMap.hpp -- Print ActvityMap Information                     */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_activityMap (
  "Prompt_activityMap"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the functopm
  "activityMap"_Stz,        // the prompt command
  nullptr,                  // the second word
  bPrompt_activityMap,      // prompt function
  true,                     // enabled

  true,                     // visible
  "fun help"_Stz            // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_activityMap(SePrompt* pPrompt)
{
  ActivityMap::vWriteCsv();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
