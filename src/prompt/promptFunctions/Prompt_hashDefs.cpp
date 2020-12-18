/*============================================================================*/
/* Prompt_hashDefs.cpp -- hash define report                                  */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_hashDefs (
  PF_HASHDEFS,          // prompt function
  "Prompt_numbers"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "hashDefs"_Stz,       // the prompt command
  nullptr,              // the second word
  bPrompt_hashDefs,     // prompt function
  true,                 // enabled

  true,                 // visible
  "hashDefs"_Stz,       // help
  ""_Stz                // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_hashDefs(SePrompt* pPrompt)
{
  
  
  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
