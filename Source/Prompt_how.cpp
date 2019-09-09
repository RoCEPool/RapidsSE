/*============================================================================*/
/* Prompt_how.cpp -- how was a request executed                               */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_how (
  PF_HOW,               // prompt function
  "Prompt_how"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the function
  "how"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_how,          // prompt function
  true,                 // enabled

  true,                 // visible
  "how"_Stz,            // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_how(SePrompt* pPrompt)
{
  i8 iReqId = pPrompt->iRequestId();
  
  vTrace("how: request %lld\n"_Stz,
         
         iReqId,
         
         C_ENDARGS);
  
  AM::vLogHow(iReqId,C_UNKNOWN);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
