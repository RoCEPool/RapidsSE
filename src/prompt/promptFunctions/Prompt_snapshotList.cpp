/*============================================================================*/
/* Prompt_snapshotList.cpp -- objects list report                               */
/*============================================================================*/

#include "STORE.h"

PromptFun __Prompt_snapshotList (
  PF_SNAPSHOTLIST,            // prompt function
  "Prompt_snapshotList"_Stz,  // name of the prompt function
  "comment"_Stz,              // comment about the function
  "snapshotList"_Stz,         // the prompt command
  nullptr,                    // the second word
  bPrompt_snapshotList,       // prompt function
  true,                       // enabled

  true,                       // visible
  "snapshotList"_Stz,         // help
  ""_Stz                      // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotList(SePrompt* pPrompt)
{
  ObjectSnapshot* pSnapshotObjects = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects)
    return true;
  
  pSnapshotObjects->vPrintCounts();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
