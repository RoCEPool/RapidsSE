/*============================================================================*/
/* Prompt_snapshotInspect.cpp -- snapshot inspect                             */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


PromptFun __Prompt_snapshotInspect (
  PF_SNAPSHOTINSPECT,            // prompt function
  "Prompt_snapshotInspect"_Stz,  // name of the prompt function
  "comment"_Stz,                 // comment about the function
  "snapshotInspect"_Stz,         // the prompt command
  nullptr,                       // the second word
  bPrompt_snapshotInspect,       // prompt function
  true,                          // enabled

  true,                          // visible
  "snapshotInspect"_Stz,         // help
  ""_Stz                         // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotInspect(SePrompt* pPrompt)
{
  ObjectSnapshot* pSnapshotObjects_then = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects_then)
    return true;
    
  //pSnapshotObjects_then->vDeltaList(pSnapshotObjects_curr);
   
  i8 iAfter = pSnapshotObjects_then->m_iWhenSnapshot;
  
  PoolHelper::vInspectObjects(iAfter);
  
  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
