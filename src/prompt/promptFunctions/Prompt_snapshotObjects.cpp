/*============================================================================*/
/* Prompt_snapshotObjects.cpp -- snapshot objects                             */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


PromptFun __Prompt_snapshotObjects (
  PF_SNAPSHOTOBJECTS,            // prompt function
  "Prompt_snapshotObjects"_Stz,  // name of the prompt function
  "comment"_Stz,                 // comment about the function
  "snapshotObjects"_Stz,         // the prompt command
  nullptr,                       // the second word
  bPrompt_snapshotObjects,       // prompt function
  true,                          // enabled

  true,                          // visible
  "snapshotObjects"_Stz,         // help
  ""_Stz                         // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotObjects(SePrompt* pPrompt)
{
  ObjectSnapshot* pSnapshotObjects = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects) {
    pSnapshotObjects = new ObjectSnapshot;
    pPrompt->vSnapshotObjects(pSnapshotObjects);
  }
  
  pSnapshotObjects->vTakeSnapshot();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
