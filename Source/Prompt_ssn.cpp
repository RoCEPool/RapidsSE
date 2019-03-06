/*============================================================================*/
/* Prompt_ssn.hpp -- Print Session Information                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_ssn (
  PF_SSN,                 // prompt function
  "Prompt_ssn"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "ssn"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_ssn,            // prompt function
  true,                   // enabled

  true,                   // visible
  "ssn"_Stz,               // help
                            ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ssn(SePrompt* pPrompt)
{
  Token* pArg0 = nullptr;
  Token* pArg1 = nullptr;

  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 nTokens = pTokenVec->nItems();

  if (nTokens != 3)
    return false;

  pArg0 = pTokenVec->pGet(1);
  pArg1 = pTokenVec->pGet(2);

  s32 sNumber;
  sNumber.vConcat(pArg1->aText());
  vTrace("Session: %s\n"_Stz,sNumber.aStz());

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
