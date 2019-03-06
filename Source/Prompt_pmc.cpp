/*============================================================================*/
/* Prompt_pmc.cpp -- perfromance monitor counters                             */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_pmc (
  PF_PMC,               // prompt function
  "Prompt_pmc"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "pmc"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_pmc,          // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_pmc(SePrompt* pPrompt)
{
  i8 iPmc = 0; //iPmc_Info();
  
  vTrace("pmc: %lld\n"_Stz,iPmc);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
