/*============================================================================*/
/* Prompt_args.cpp -- show command line arguments                             */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_args (
  PF_ARGS,           // prompt function
  "Prompt_args"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "args"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_args,      // prompt function
  true,              // enabled

  true,               // visible
  "fun help"_Stz      // help
);

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_args(SePrompt* pPrompt)
{
  gxArguments.vPrint();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
