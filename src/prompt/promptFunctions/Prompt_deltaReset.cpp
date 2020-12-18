/*============================================================================*/
/* Prompt_deltaReset.cpp -- object delta reset                                */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_deltaReset (
  PF_DELTARESET,            // prompt function
  "Prompt_deltaReset"_Stz,  // name of the prompt function
  "comment"_Stz,            // comment about the function
  "deltaReset"_Stz,         // the prompt command
  nullptr,                  // the second word
  bPrompt_deltaReset,       // prompt function
  true,                     // enabled

  true,                     // visible
  "deltaReset"_Stz,         // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_deltaReset(SePrompt* pPrompt)
{
  pPrompt->pBPA()->vReset();
    
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
