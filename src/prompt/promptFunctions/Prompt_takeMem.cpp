/*============================================================================*/
/* Prompt_takeMem.cpp -- take heap memory                                     */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_takeMem (
  PF_TAKEMEM,           // prompt function
  "Prompt_takeMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "takeMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_takeMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "takeMem"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_takeMem(SePrompt* pPrompt)
{
  i8 nSpare = 0;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 iNumTokens = pInpTknVec->nItems();
  if (2 == iNumTokens) {
    if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
      return false;
    
    // get the no of indexEntries to set
    nSpare = pInpTknVec->pGet(1)->iToI8();
  }
  
  while (gpBufferPool->nFreeCount() >= C_LOW_BUFFERS + nSpare) {

    PoolBuffer* p2ndBuffer  = AM::pTakenBuffers();
    AM::vTakenBuffers(gpBufferPool->pAllocOtherBuffer());
    
    PoolBuffer** ppNextBuffer = (PoolBuffer**)AM::pTakenBuffers()->aDataArea();
    *ppNextBuffer = p2ndBuffer;
    
    AM::vTakenBytes(C_SIZE_POOL_BUFFER);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
