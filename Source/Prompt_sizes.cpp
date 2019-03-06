/*============================================================================*/
/* Prompt_sizes.cpp -- sizes report                                           */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_sizes (
  PF_SIZES,             // prompt function
  "Prompt_sizes"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "sizes"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_sizes,        // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
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
  vTrace("  sizeof(BolVal) = %lu; \n"_Stz, sizeof(BolVal));
  vTrace("  sizeof(DteVal) = %lu; \n"_Stz, sizeof(DteVal));
  vTrace("  sizeof(FltVal) = %lu; \n"_Stz, sizeof(FltVal));
  vTrace("  sizeof(IntVal) = %lu; \n"_Stz, sizeof(IntVal));
  vTrace("  sizeof(IvlVal) = %lu; \n"_Stz, sizeof(IvlVal));
  vTrace("  sizeof(PtrVal) = %lu; \n"_Stz, sizeof(PtrVal));
  vTrace("  sizeof(StrVal) = %lu; \n"_Stz, sizeof(StrVal));
  vTrace("  sizeof(TmeVal) = %lu; \n"_Stz, sizeof(TmeVal));
  vTrace("  sizeof(TspVal) = %lu; \n"_Stz, sizeof(TspVal));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(Task)   = %lu; \n"_Stz, sizeof(Task));
  vTrace("  sizeof(Stack)  = %lu; \n"_Stz, sizeof(Stack));
  vTrace("  Task+Stacks    = %lu; \n"_Stz, sizeof(Stack) + 3 * sizeof(Stack));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(s32)    = %lu; \n"_Stz, sizeof(s32));
  vTrace("  sizeof(s256)   = %lu; \n"_Stz, sizeof(s256));
  vTrace("  sizeof(s4K)    = %lu; \n"_Stz, sizeof(s4K));
  vTrace("  sizeof(s16KB)  = %lu; \n"_Stz, sizeof(s16KB));
  vTrace("  sizeof(s17KB)  = %lu; \n"_Stz, sizeof(s17KB));
  vTrace("  sizeof(s48K)   = %lu; \n"_Stz, sizeof(s48K));
  vTrace("  sizeof(s1M)    = %lu; \n"_Stz, sizeof(s1M));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(JobDicPath)    = %lu; \n"_Stz, sizeof(JobDicPath));
  vTrace("  sizeof(JobFilePath)   = %lu; \n"_Stz, sizeof(JobFilePath));
  vTrace("  sizeof(JobResult)     = %lu; \n"_Stz, sizeof(JobResult));
  vTrace("  sizeof(JobObjectName) = %lu; \n"_Stz, sizeof(JobObjectName));
  vTrace("\n"_Stz);

  vTrace("  sizeof(FreeChunk)    = %lu; \n"_Stz, sizeof(FreeChunk));
  vTrace("  sizeof(ApiState)     = %lu; \n"_Stz, sizeof(ApiState));
  vTrace("  sizeof(ActivityMap)  = %lu; \n"_Stz, sizeof(ActivityMap));
  vTrace("  sizeof(AttachMap)    = %lu; \n"_Stz, sizeof(AttachMap));
  vTrace("  sizeof(SessionState) = %lu; \n"_Stz, sizeof(SessionState));
  
  vTrace("\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
