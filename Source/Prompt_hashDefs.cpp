/*============================================================================*/
/* Prompt_hashDefs.cpp -- hash define report                                  */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_hashDefs (
  "Prompt_numbers"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "hashDefs"_Stz,       // the prompt command
  nullptr,              // the second word
  bPrompt_hashDefs,     // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_hashDefs(SePrompt* pPrompt)
{
  vTrace("\n"_Stz);

#ifdef __APPLE__
  vTrace("  __APPLE__\n"_Stz);
#endif
  
#ifdef WITH_ADDRESS_CHECK
  vTrace("  WITH_ADDRESS_CHECK\n"_Stz);
#endif
  
#ifdef WITH_ASSERT_CHECKS
  vTrace("  WITH_ASSERT_CHECKS\n"_Stz);
#endif
  
#ifdef WITH_CLASS_CHECK
  vTrace("  WITH_CLASS_CHECK\n"_Stz);
#endif
  
#ifdef __DEBUG
  vTrace("  __DEBUG\n"_Stz);
#endif
  
#ifdef WITH_DEBUG_CHECKS
  vTrace("  WITH_DEBUG_CHECKS\n"_Stz);
#endif
  
#ifdef WITH_ENTRY_EXIT
  vTrace("  WITH_ENTRY_EXIT\n"_Stz);
#endif
  
#ifdef WITH_FOOTER_CHECK
  vTrace("  WITH_FOOTER_CHECK\n"_Stz);
#endif
  
#ifdef WITH_MAGIC_CHECK
  vTrace("  WITH_MAGIC_CHECK\n"_Stz);
#endif
  
#ifdef WITH_MEMORY_ZERO
  vTrace("  WITH_MEMORY_ZERO\n"_Stz);
#endif
  
#ifdef _DEBUG
  vTrace("  _DEBUG\n"_Stz);
#endif
  
#ifdef _VLQ
  vTrace("  _VLQ\n"_Stz);
#endif
  
#ifdef EB_LAZY_CHAIN
  vTrace("  EB_LAZY_CHAIN\n"_Stz);
#endif
  
#ifdef EB_SINGLE_COLIDX
  vTrace("  EB_SINGLE_COLIDX\n"_Stz);
#endif
  
#ifdef EB_SPLIT2
  vTrace("  EB_SPLIT2\n"_Stz);
#endif
  
#ifdef IT_COMPACT_P1_TRACE
  vTrace("  IT_COMPACT_P1_TRACE\n"_Stz);
#endif
  
#ifdef IT_COPYLEFTV3
  vTrace("  IT_COPYLEFTV3\n"_Stz);
#endif
  
#ifdef WITH_JOB_TRACE
  vTrace("  WITH_JOB_TRACE %lld\n"_Stz,(i8)WITH_JOB_TRACE);
#endif
  
#ifdef JUMP_FALSE
  vTrace("  JUMP_FALSE\n"_Stz);
#endif
  
#ifdef WITH_LW_FOOTER
  vTrace("  WITH_LW_FOOTER\n"_Stz);
#endif
  
#ifdef MB_WATCH
  vTrace("  MB_WATCH\n"_Stz);
#endif
  
#ifdef MP_CHECK
  vTrace("  MP_CHECK\n"_Stz);
#endif
  
#ifdef WITH_MUNGED
  vTrace("  WITH_MUNGED\n"_Stz);
#endif
    
#ifdef STRING_LENGTH_TEST
  vTrace("  STRING_LENGTH_TEST\n"_Stz);
#endif
  
  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
