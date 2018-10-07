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
  
  while (gpBufferPool->iFreeCount() >= C_LOW_BUFFERS + 5) {

    Buffer* p2ndBuffer  = gpTakenBuffers;
    gpTakenBuffers = gpBufferPool->pAllocBuffer();
    
    Buffer** ppNextBuffer = (Buffer**)gpTakenBuffers->aDataArea();
    *ppNextBuffer = p2ndBuffer;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
