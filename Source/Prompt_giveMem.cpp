/*============================================================================*/
/* Prompt_giveMem.cpp -- give heap memory                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_giveMem (
  "Prompt_giveMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "giveMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_giveMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern Buffer* gpTakenBuffers;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_giveMem(SePrompt* pPrompt)
{
  while (gpTakenBuffers) {
    
    Buffer* pBuffer     = gpTakenBuffers;
    Buffer* pNextBuffer = *(Buffer**)pBuffer->aDataArea();

    gpTakenBuffers = pNextBuffer;
    
    gpBufferPool->vFree(pBuffer);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
