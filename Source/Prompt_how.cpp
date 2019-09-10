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
  TokenVec* pTokenVec = pPrompt->pTknVec();
  
  i8 nTokens = pTokenVec->nItems();

  i8 iReqId = pPrompt->iRequestId();
  
  i8 iDetail = C_UNKNOWN;
  if (2 == nTokens)
    iDetail = pTokenVec->pGet(1)->iToI8();
  
  vTrace("how: request %lld\n"_Stz,
         
         iReqId,
         
         C_ENDARGS);
  
  AM::vLogHow(iReqId,iDetail);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
