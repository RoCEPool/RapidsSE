/*============================================================================*/
/* Prompt_args.cpp -- show command line arguments                             */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_args (
  "Prompt_args"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "args"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_args,      // prompt post-process function
  true               // enabled
);

int    gi_argc;
char** gi_argv;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_args(SePrompt* pPrompt)
{
  vTrace("\nArgs:\n"_Stz);
  for (i8 i_Arg = 0; i_Arg < gi_argc; i_Arg++) {
    vTrace("  %s\n"_Stz,gi_argv[i_Arg]);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
