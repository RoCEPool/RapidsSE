/*============================================================================*/
/* Prompt_sizes.cpp -- sizes report                                           */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_sizes (
  PF_SIZES,             // prompt function
  "Prompt_sizes"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "sizes"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_sizes,        // prompt function
  true,                 // enabled

  true,                 // visible
  "sizes"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_sizes(SePrompt* pPrompt)
{
  vTrace("  sizeof(Header) = %lu; \n"_Stz, sizeof(Header));
  vTrace("  sizeof(Object) = %lu; \n"_Stz, sizeof(Object));
  vTrace("  sizeof(Footer) = %lu; \n"_Stz, sizeof(Footer));

  vTrace("\n"_Stz);
  vTrace("  sizeof(PoolBuffer) = %lu; \n"_Stz, sizeof(PoolBuffer));
  
  vTrace("\n"_Stz);
  vTrace("  sizeof(IntVal) = %lu; \n"_Stz, sizeof(IntVal));
  vTrace("  sizeof(PtrVal) = %lu; \n"_Stz, sizeof(PtrVal));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(Task)   = %lu; \n"_Stz, sizeof(Task));
  vTrace("  sizeof(Stack)  = %lu; \n"_Stz, sizeof(Stack));
  vTrace("  Task+Stacks    = %lu; \n"_Stz, sizeof(Stack) + 3 * sizeof(Stack));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(s32)    = %lu; \n"_Stz, sizeof(s32));
  vTrace("  sizeof(s256)   = %lu; \n"_Stz, sizeof(s256));
  vTrace("  sizeof(s4K)    = %lu; \n"_Stz, sizeof(s4K));
  vTrace("  sizeof(s16KB)  = %lu; \n"_Stz, sizeof(s16KB));
  vTrace("  sizeof(s24KB)  = %lu; \n"_Stz, sizeof(s24KB));
  vTrace("  sizeof(s48K)   = %lu; \n"_Stz, sizeof(s48K));
  vTrace("  sizeof(s1M)    = %lu; \n"_Stz, sizeof(s1M));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(JobFilePath)   = %lu; \n"_Stz, sizeof(JobFilePath));
  vTrace("\n"_Stz);

  vTrace("  sizeof(FreeChunk)    = %lu; \n"_Stz, sizeof(FreeChunk));
  vTrace("  sizeof(SessionArray) = %lu; \n"_Stz, sizeof(SessionArray));
  vTrace("\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
