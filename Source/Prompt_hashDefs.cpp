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
  
#ifdef _DEBUG
  vTrace("  _DEBUG\n"_Stz);
#endif
  
#ifdef __DEBUG
  vTrace("  __DEBUG\n"_Stz);
#endif
  
#ifdef WITH_ADDRESS_CHECKING
  vTrace("  WITH_ADDRESS_CHECKING\n"_Stz);
#endif
  
#ifdef WITH_ASSERT_MACRO
  vTrace("  WITH_ASSERT_MACRO\n"_Stz);
#endif
  
#ifdef WITH_CORECTNESS_ALGEBRA
  vTrace("  WITH_CORECTNESS_ALGEBRA\n"_Stz);
#endif
  
#ifdef WITH_ENTRY_EXIT
  vTrace("  WITH_ENTRY_EXIT\n"_Stz);
#endif
  
#ifdef WITH_JOB_TRACE
  vTrace("  WITH_JOB_TRACE %lld\n"_Stz,(i8)WITH_JOB_TRACE);
#endif
  
#ifdef WITH_LW_FOOTER
  vTrace("  WITH_LW_FOOTER\n"_Stz);
#endif
  
#ifdef WITH_MB_WATCH
  vTrace("  WITH_MB_WATCH\n"_Stz);
#endif
  
#ifdef WITH_MUNGED
  vTrace("  WITH_MUNGED\n"_Stz);
#endif
  
#ifdef WITH_PREPOST_CHECKING
  vTrace("  WITH_PREPOST_CHECKING\n"_Stz);
#endif
  
#ifdef WITH_VLQ
  vTrace("  WITH_VLQ\n"_Stz);
#endif
  
  vTrace("\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  
  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
