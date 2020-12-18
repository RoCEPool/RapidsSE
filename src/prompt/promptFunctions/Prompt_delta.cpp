/*============================================================================*/
/* Prompt_delta.cpp -- objec delta report                                     */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_delta (
  PF_DELTA,            // prompt function
  "Prompt_delta"_Stz,  // name of the prompt function
  "comment"_Stz,       // comment about the function
  "delta"_Stz,         // the prompt command
  nullptr,             // the second word
  bPrompt_delta,       // prompt function
  true,                // enabled

  true,                // visible
  "delta"_Stz,         // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_delta(SePrompt* pPrompt)
{
  s48K sReport;
  pPrompt->pBPA()->vDeltaReport(sReport);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Object Delta Report\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  vTrace("%s"_Stz,sReport.szStz());
  vTrace("====================================\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
