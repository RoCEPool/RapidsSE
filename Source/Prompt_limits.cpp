/*============================================================================*/
/* Prompt_limits.cpp -- system limits                                         */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_limits (
  PF_LIMITS,            // prompt function
  "Prompt_limits"_Stz,  // name of the prompt function
  "comment"_Stz,        // comment about the function
  "limits"_Stz,         // the prompt command
  nullptr,              // the second word
  bPrompt_limits,       // prompt function
  true,                 // enabled

  true,                 // visible
  "numbers"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_limits(SePrompt* pPrompt)
{
  vTrace("\n"_Stz);
  
  vTrace("  C_MAX_COLUMNS  = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_RECORD   = %lldB\n"_Stz,C_MAX_RECORD);
  vTrace("  C_MAX_VCH      = %lld\n"_Stz,C_MAX_VCH);

  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
