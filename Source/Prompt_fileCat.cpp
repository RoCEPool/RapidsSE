/*============================================================================*/
/* Prompt_fileCat.cpp -- file cat                                             */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_fileCat (
  PF_FILECAT,           // prompt function
  "Prompt_fileCat"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "fileCat"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_fileCat,      // prompt function
  true,                 // enabled

  true,              // visible
  "fileCat"_Stz,     // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_fileCat(SePrompt* pPrompt)
{
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
