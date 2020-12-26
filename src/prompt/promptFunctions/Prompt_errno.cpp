/*============================================================================*/
/* Prompt_errno.cpp -- errno print                                            */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_errno (
  PF_ERRNO,             // prompt function
  "Prompt_errno"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "errno"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_errno,        // prompt function
  true,                 // enabled

  true,                 // visible
  "errno"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_errno(SePrompt* pPrompt)
{
  i8 iErrNo = 0;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens) {
    if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
      return false;
    
    // get the no of indexEntries to set
    iErrNo = pInpTknVec->pGet(1)->iToI8();

    vTrace("  %s\n"_Stz,strerror((int)iErrNo));
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (iErrNo = 0; iErrNo < 128; iErrNo++) {
    if (StrLib::bContains(strerror((int)iErrNo),"Unknown error"))
      break;
    vTrace("  %lld %s\n"_Stz,iErrNo,strerror((int)iErrNo));
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
