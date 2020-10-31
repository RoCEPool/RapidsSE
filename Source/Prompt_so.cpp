/*============================================================================*/
/* Prompt_so.cpp -- print so information                                      */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_so (
  PF_SO,           // prompt function
  "Prompt_so"_Stz, // name of the prompt function
  "comment"_Stz,   // comment about the function
  "so"_Stz,        // the prompt command
  nullptr,         // the second word
  bPrompt_so,      // prompt function
  true,            // enabled

  true,                  // visible
  "so"_Stz,              // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_so(SePrompt* pPrompt)
{
  SP_Gid spId;
  
  //vTrace(" Request Id : %s\n"_Stz,spId.sz(IK_PROMPT,pPrompt->iRequestId()));
         
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
