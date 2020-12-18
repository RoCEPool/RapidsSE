/*============================================================================*/
/* Prompt_objects.cpp -- objects list report                                  */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_objects (
  PF_OBJECTS,            // prompt function
  "Prompt_objects"_Stz,  // name of the prompt function
  "comment"_Stz,         // comment about the function
  "objects"_Stz,         // the prompt command
  nullptr,               // the second word
  bPrompt_objects,       // prompt function
  true,                  // enabled

  true,                  // visible
  "objects"_Stz,         // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_objects(SePrompt* pPrompt)
{
  s48K sReport;
  pPrompt->pBPA()->vObjectReport(sReport);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Current Object Report\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  vTrace("%s"_Stz,sReport.szStz());
  vTrace("====================================\n"_Stz);
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
