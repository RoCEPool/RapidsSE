/*============================================================================*/
/* Prompt_snapshotNewer.cpp -- snapshot newer                                 */
/*============================================================================*/

#include "STORE.h"

PromptFun __Prompt_snapshotNewer (
  PF_SNAPSHOTNEWER,            // prompt function
  "Prompt_snapshotNewer"_Stz,  // name of the prompt function
  "comment"_Stz,               // comment about the function
  "snapshotNewer"_Stz,         // the prompt command
  nullptr,                     // the second word
  bPrompt_snapshotNewer,       // prompt function
  true,                        // enabled

  true,                        // visible
  "snapshotNewer"_Stz,         // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotNewer(SePrompt* pPrompt)
{
  ObjectSnapshot* pSnapshotObjects_then = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects_then)
    return true;
    
  //pSnapshotObjects_then->vDeltaList(pSnapshotObjects_curr);
   
  i8 iAfter = pSnapshotObjects_then->m_iWhenSnapshot;
  
  PoolHelper::vObjectDump(iAfter);
  
  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
