/*============================================================================*/
/* Prompt_takeMem.cpp -- take heap memory                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_takeMem (
  "Prompt_takeMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "takeMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_takeMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

Buffer* gpTakenBuffers = nullptr;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_takeMem(SePrompt* pPrompt)
{
  i8 nSpare = 0;

  TokenVec* pTokenVec = pPrompt->pTknVec();

  i8 iNumTokens = pTokenVec->nItems();
  if (2 == iNumTokens) {
    if (pTokenVec->pGet(1)->eToken() != TN_INTEGER)
      return false;
    
    // get the no of indexEntries to set
    nSpare = pTokenVec->pGet(1)->iToI8();
  }
  
  while (gpBufferPool->nFreeCount() >= C_LOW_BUFFERS + nSpare) {

    Buffer* p2ndBuffer  = gpTakenBuffers;
    gpTakenBuffers = gpBufferPool->pAllocOtherBuffer();
    
    Buffer** ppNextBuffer = (Buffer**)gpTakenBuffers->aDataArea();
    *ppNextBuffer = p2ndBuffer;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
