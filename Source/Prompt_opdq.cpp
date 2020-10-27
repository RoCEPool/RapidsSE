/*============================================================================*/
/* Prompt_opdq.cpp -- Operand Queue Repo                                      */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_opdq (
  PF_OPDQ,              // prompt function
  "Prompt_opdq"_Stz,    // name of the prompt function
  "comment"_Stz,        // comment about the function
  "opdq"_Stz,           // the prompt command
  nullptr,              // the second word
  bPrompt_opdq,         // prompt function
  true,                 // enabled

  true,                 // visible
  "opdq"_Stz,           // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_opdq(SePrompt* pPrompt)
{
  vTrace("Operand Queue Report\n"_Stz);

  Engine::vOperandQueueReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
