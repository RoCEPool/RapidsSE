/*============================================================================*/
/* Prompt_objectValidate.cpp -- object validation                             */
/*============================================================================*/

#include "STORE.h"

PromptFun __Prompt_objectValidate (
  PF_OBJECTVALIDATE,            // prompt function
  "Prompt_objectValidate"_Stz,  // name of the prompt function
  "comment"_Stz,                // comment about the function
  "objectValidate"_Stz,         // the prompt command
  nullptr,                      // the second word
  bPrompt_objectValidate,       // prompt function
  true,                         // enabled

  true,                         // visible
  "objectValidate"_Stz,         // help
  ""_Stz                        // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_objectValidate(SePrompt* pPrompt)
{
  PoolHelper::vObjectValidate();
  
  return true;
}
/*----------------------------------------------------------------------------*/
//bool PROMPT::bPrompt_objectValidate(SePrompt* pPrompt)
//{
//  s48K sReport;
//  pPrompt->pBPA()->vObjectReport(sReport);
//
//  vTrace("------------------------------------\n"_Stz);
//  vTrace("Current Object Report\n"_Stz);
//  vTrace("------------------------------------\n"_Stz);
//
//  vTrace("%s"_Stz,sReport.szStz());
//  vTrace("====================================\n"_Stz);
//  return true;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
