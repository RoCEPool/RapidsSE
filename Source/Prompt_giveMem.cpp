/*============================================================================*/
/* Prompt_giveMem.cpp -- give heap memory                                     */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*   gpAttachMap;

PromptNote mPromptNote_giveMem (
  PF_GIVEMEM,           // prompt function
  "Prompt_giveMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "giveMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_giveMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "giveMem"_Stz         // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_giveMem(SePrompt* pPrompt)
{
  while (gpAttachMap->m_pTakenBuffers) {
    
    PoolBuffer* pBuffer     = gpAttachMap->m_pTakenBuffers;
    PoolBuffer* pNextBuffer = *(PoolBuffer**)pBuffer->aDataArea();

    gpAttachMap->m_pTakenBuffers = pNextBuffer;
    
    gpBufferPool->vFree(pBuffer);
  }
  
  gpAttachMap->m_nTakenBytes = 0;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
