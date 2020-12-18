/*============================================================================*/
/* Prompt_how.cpp -- how was a request executed                               */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_how (
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
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 nTokens = pInpTknVec->nItems();

  //i8 iReqId = pPrompt->iRequestId();
  
  i8 iDetail = C_UNKNOWN;
  if (2 == nTokens)
    iDetail = pInpTknVec->pGet(1)->iToI8();
  
  vTrace("how: request\n"_Stz,
         
         //iReqId,
         
         C_VARGS_END);
  
  AM::vLogHow(-1,iDetail);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
