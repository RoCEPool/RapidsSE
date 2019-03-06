/*============================================================================*/
/* Prompt_rassert.cpp -- execute an rassert(0)                                */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_rassert (
  PF_RASSERT,          // prompt function
  "Prompt_rassert"_Stz,// name of the prompt function
  "comment"_Stz,       // comment about the functopm
  "rassert"_Stz,       // the prompt command
  nullptr,             // the second word
  bPrompt_print,       // prompt function
  true,                // enabled

  true,                // visible
  "fun help"_Stz       // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_rassert(SePrompt* pPrompt)
{
  RASSERT(0);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
