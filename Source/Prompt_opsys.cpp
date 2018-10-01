/*============================================================================*/
/* Prompt_opsys.cpp -- Print OS Information                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_opsys (
  "Prompt_opsys"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "opsys"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_opsys,      // prompt post-process function
  true                // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_opsys(SePrompt* pPrompt)
{
  
  OpSys::vGetInfo();
  OpSys::vTraceInfo();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
