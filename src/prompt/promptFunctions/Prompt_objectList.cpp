/*============================================================================*/
/* Prompt_objectList.cpp -- objects list report                               */
/*============================================================================*/

#include "STORE.h"


PromptFun __Prompt_objectList (
  PF_OBJECTLIST,            // prompt function
  "Prompt_objectList"_Stz,  // name of the prompt function
  "comment"_Stz,            // comment about the function
  "- objectList"_Stz,         // the prompt command
  nullptr,                  // the second word
  bPrompt_objectList,       // prompt function
  true,                     // enabled

  true,                     // visible
  "objectList"_Stz,         // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_objectList(SePrompt* pPrompt)
{
  
  ObjectSnapshot* pObjectSnapshot = new ObjectSnapshot;
  
  pObjectSnapshot->vTakeSnapshot();
  
  pObjectSnapshot->vPrintCounts();

  delete pObjectSnapshot;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
