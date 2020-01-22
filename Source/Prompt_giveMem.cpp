/*============================================================================*/
/* Prompt_giveMem.cpp -- give heap memory                                     */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_giveMem (
  PF_GIVEMEM,           // prompt function
  "Prompt_giveMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "giveMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_giveMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "giveMem"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_giveMem(SePrompt* pPrompt)
{
  while (AM::pTakenBuffers()) {
    
    PoolBuffer* pBuffer     = AM::pTakenBuffers();
    PoolBuffer* pNextBuffer = *(PoolBuffer**)pBuffer->aDataArea();

    AM::vTakenBuffers(pNextBuffer);
    
    gpBufferPool->vFree(pBuffer);
  }
  
  AM::vTakenBytes(0);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
