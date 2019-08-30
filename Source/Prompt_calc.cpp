/*============================================================================*/
/* Prompt_calc.cpp -- calc                                                    */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_calc (
  PF_BYE,            // prompt function
  "Prompt_calc"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "calc"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_calc,      // prompt function
  true,              // enabled

  true,              // visible
  "calc"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_calc(SePrompt* pPrompt)
{

  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
