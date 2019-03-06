/*============================================================================*/
/* Prompt_errno.cpp -- errno print                                            */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_errno (
  PF_ERRNO,             // prompt function
  "Prompt_errno"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "errno"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_errno,        // prompt function
  true,                 // enabled

  true,                 // visible
  "errno"_Stz,           // help
                              ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_errno(SePrompt* pPrompt)
{
  i8 iErrNo = 0;

  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 iNumTokens = pTokenVec->nItems();
  
  if (2 == iNumTokens) {
    if (pTokenVec->pGet(1)->eToken() != TN_INTEGER)
      return false;
    
    // get the no of indexEntries to set
    iErrNo = pTokenVec->pGet(1)->iToI8();
  }
  
  vTrace("  %s\n"_Stz,strerror((int)iErrNo));
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
