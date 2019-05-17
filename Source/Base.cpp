/*============================================================================*/
/* Base.cpp - Base namespace functions                                        */
/*============================================================================*/

#include "Base.h"
#include "StorageEngine.h"
/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Base::vCheckAssumptions(void)
{
  RASSERT(sizeof(Word)       == C_BYTES_PER_WORD);
  RASSERT(sizeof(DoubleWord) == 2 * C_BYTES_PER_WORD);
  RASSERT(sizeof(QuadWord)   == 4 * C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* trace with                                                                 */
/*----------------------------------------------------------------------------*/
void Base::vTraceWith(void)
{
#ifdef WITH_ACTION_SEQUENCE
  vTrace("  WITH_ACTION_SEQUENCE\n"_Stz);
#endif

#ifdef WITH_ACTIVITY_MAP
  vTrace("  WITH_ACTIVITY_MAP\n"_Stz);
#endif

#ifdef WITH_ADDRESS_CHECKING
  vTrace("  WITH_ADDRESS_CHECKING\n"_Stz);
#endif
  
#ifdef WITH_APPLE
  vTrace("  WITH_APPLE\n"_Stz);
#endif
  
#ifdef WITH_ASSERT_MACRO
  vTrace("  WITH_ASSERT_MACRO\n"_Stz);
#endif
  
#ifdef WITH_CHECK_INITIALISED
  vTrace("  WITH_CHECK_INITIALISED\n"_Stz);
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
  
#ifdef WITH_LINUX
  vTrace("  WITH_LINUX\n"_Stz);
#endif
  
#ifdef WITH_MUNGED_SEN
  vTrace("  WITH_MUNGED_SEN\n"_Stz);
#endif

#ifdef WITH_PREPOST_CHECKING
  vTrace("  WITH_PREPOST_CHECKING\n"_Stz);
#endif
  
#ifdef WITH_RELEASE_VERSION
  vTrace("  WITH_RELEASE_VERSION\n"_Stz);
#endif
  
#ifdef WITH_TRACE_API
  vTrace("  WITH_TRACE_API\n"_Stz);
#endif
  
#ifdef WITH_TRACE_ENCODING
  vTrace("  WITH_TRACE_ENCODING\n"_Stz);
#endif
  
#ifdef WITH_ENCODING_XCHECK
  vTrace("  WITH_ENCODING_XCHECK\n"_Stz);
#endif
  
#ifdef WITH_EXTENDED_BCD
  vTrace("  WITH_EXTENDED_BCD\n"_Stz);
#endif
  
#ifdef WITH_EXTENDED_DTE
  vTrace("  WITH_EXTENDED_DTE\n"_Stz);
#endif
  
#ifdef WITH_EXTENDED_INT_DIGIT
  vTrace("  WITH_EXTENDED_INT_DIGIT\n"_Stz);
#endif
  
#ifdef WITH_EXTENDED_VCH
  vTrace("  WITH_EXTENDED_VCH\n"_Stz);
#endif
  
#ifdef WITH_LARGE_ROWS
  vTrace("  WITH_LARGE_ROWS\n"_Stz);
#endif
  
#ifdef WITH_SHOCO_DUPLICATES
  vTrace("  WITH_SHOCO_DUPLICATES\n"_Stz);
#endif
  
#ifdef WITH_SHOCO_FOLDLAST
  vTrace("  WITH_SHOCO_FOLDLAST\n"_Stz);
#endif


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
