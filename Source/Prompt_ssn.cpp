/*============================================================================*/
/* Prompt_ssn.hpp -- Print Session Information                                */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_ssn (
  PF_SSN,                 // prompt function
  "Prompt_ssn"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "ssn"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_ssn,            // prompt function
  true,                   // enabled

  true,                   // visible
  "ssn"_Stz,              // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ssn(SePrompt* pPrompt)
{
  Token* pArg0 = nullptr;
  Token* pArg1 = nullptr;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pInpTknVec->nItems();

  if (nTokens != 3)
    return false;

  pArg0 = pInpTknVec->pGet(1);
  pArg1 = pInpTknVec->pGet(2);

  s32 sNumber;
  sNumber.vConcat(pArg1->szChars());
  vTrace("Session: %s\n"_Stz,sNumber.szStz());

  i8 iNumber = sNumber.iToI8();
  
  Session* pSession = _pSession(iNumber);
  
  if (! pSession) {
    vTrace("Not a session\n"_Stz);
    return false;
  }
    
  pSession->vReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
