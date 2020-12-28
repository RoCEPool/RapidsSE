/*============================================================================*/
/* Prompt_snapshotDelta.cpp -- snapshot delta                                 */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


PromptFun __Prompt_snapshotDelta (
  PF_SNAPSHOTDELTA,            // prompt function
  "Prompt_snapshotDelta"_Stz,  // name of the prompt function
  "comment"_Stz,               // comment about the function
  "snapshotDelta"_Stz,         // the prompt command
  nullptr,                     // the second word
  bPrompt_snapshotDelta,       // prompt function
  true,                        // enabled

  true,                        // visible
  "snapshotDelta"_Stz,         // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotDelta(SePrompt* pPrompt)
{
  ObjectSnapshot* pSnapshotObjects_then = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects_then)
    return true;
  
  ObjectSnapshot* pSnapshotObjects_curr = new ObjectSnapshot;
  pSnapshotObjects_curr->vTakeSnapshot();
  
  pSnapshotObjects_then->vDeltaList(pSnapshotObjects_curr);
  
  delete pSnapshotObjects_curr;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
