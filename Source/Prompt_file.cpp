/*============================================================================*/
/* Prompt_file.cpp -- file statistics                                         */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_file (
  PF_FILE,           // prompt function
  "Prompt_file"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "file"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_file,      // prompt function
  true,              // enabled

  true,              // visible
  "file"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_file(SePrompt* pPrompt)
{  
  File::vStatistics();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
