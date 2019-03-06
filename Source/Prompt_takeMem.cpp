/*============================================================================*/
/* Prompt_takeMem.cpp -- take heap memory                                     */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*   gpAttachMap;

PromptNote mPromptNote_takeMem (
  PF_TAKEMEM,           // prompt function
  "Prompt_takeMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "takeMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_takeMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "takeMem"_Stz         // help
);

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

    PoolBuffer* p2ndBuffer  = gpAttachMap->m_pTakenBuffers;
    gpAttachMap->m_pTakenBuffers = gpBufferPool->pAllocOtherBuffer();
    
    PoolBuffer** ppNextBuffer = (PoolBuffer**)gpAttachMap->m_pTakenBuffers->aDataArea();
    *ppNextBuffer = p2ndBuffer;
    
    gpAttachMap->m_nTakenBytes += C_SIZE_POOL_BUFFER;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
