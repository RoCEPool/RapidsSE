/*============================================================================*/
/* Prompt_hashDefs.cpp -- hash define report                                  */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_hashDefs (
  PF_HASHDEFS,          // prompt function
  "Prompt_numbers"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "hashDefs"_Stz,       // the prompt command
  nullptr,              // the second word
  bPrompt_hashDefs,     // prompt function
  true,                 // enabled

  true,                 // visible
  "hashDefs"_Stz,       // help
  ""_Stz                // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_hashDefs(SePrompt* pPrompt)
{
#ifdef WITH_APPLE
  vTrace("  WITH_APPLE\n"_Stz);
#endif
  
#ifdef WITH_LINUX
  vTrace("  WITH_LINUX\n"_Stz);
#endif

#ifdef WITH_ACTIVITY_MAP
  vTrace("  WITH_ACTIVITY_MAP\n"_Stz);
#endif
  
#ifdef WITH_ADDRESS_CHECKING
  vTrace("  WITH_ADDRESS_CHECKING\n"_Stz);
#endif
  
#ifdef WITH_TRACE_API
  vTrace("  WITH_TRACE_API\n"_Stz);
#endif
  
#ifdef WITH_ASSERT_MACRO
  vTrace("  WITH_ASSERT_MACRO\n"_Stz);
#endif
  
#ifdef WITH_CORECTNESS_ALGEBRA
  vTrace("  WITH_CORECTNESS_ALGEBRA\n"_Stz);
#endif
  
#ifdef WITH_DEBUG_VERSION
  vTrace("  WITH_DEBUG_VERSION\n"_Stz);
#endif
  
#ifdef WITH_ENTRY_EXIT
  vTrace("  WITH_ENTRY_EXIT\n"_Stz);
#endif
        
#ifdef WITH_MUNGED_SEN
  vTrace("  WITH_MUNGED_SEN\n"_Stz);
#endif
  
#ifdef WITH_PREPOST_CHECKING
  vTrace("  WITH_PREPOST_CHECKING\n"_Stz);
#endif

  
  
#ifdef WITH_SHOCO_DUPLICATES
  vTrace("  WITH_SHOCO_DUPLICATES\n"_Stz);
#endif

#ifdef WITH_SHOCO_FOLDLAST
  vTrace("  WITH_SHOCO_FOLDLAST\n"_Stz);
#endif



  vTrace("\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
