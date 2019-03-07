/*============================================================================*/
/* Prompt_opsys.cpp -- Print OS Information                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_opsys (
  PF_OPSYS,           // prompt function
  "Prompt_opsys"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the functopm
  "opsys"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_opsys,      // prompt function
  true,               // enabled

  true,               // visible
  "opsys"_Stz,        // help
  ""_Stz              // description
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
