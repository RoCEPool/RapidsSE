/*============================================================================*/
/* Correctness.cpp -- Correctness Algebra namespace functions                 */
/*============================================================================*/

#include "Correctness.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Correctness::vCheckAssumptions(void)
{
  ASSERT(sizeof(u2)  == sizeof(eCN));
    
  ASSERT(8 == sizeof(Header));
  ASSERT(8 == sizeof(Object));
  ASSERT(8 == sizeof(Footer));
}

/*----------------------------------------------------------------------------*/
/* trace with                                                                 */
/*----------------------------------------------------------------------------*/
void Correctness::vTraceWith(void)
{
  vTrace("\nWith:\n"_Stz);

#ifdef WITH_ACTION_SEQUENCE
  vTrace("  WITH_ACTION_SEQUENCE\n"_Stz);
#endif
  
#ifdef WITH_SHARED_LOG
  vTrace("  WITH_SHARED_LOG\n"_Stz);
#endif
  
#ifdef WITH_ADDRESS_CHECKING
  vTrace("  WITH_ADDRESS_CHECKING\n"_Stz);
#endif
  
#ifdef WITH_APPLE
  vTrace("  WITH_APPLE\n"_Stz);
#endif
  
#ifdef WITH_ANALYSE_FREE
  vTrace("  WITH_ANALYSE_FREE\n"_Stz);
#endif
  
#ifdef WITH_ASSERT_MACRO
  vTrace("  WITH_ASSERT_MACRO\n"_Stz);
#endif
  
#ifdef WITH_BUCKET_HASHING
  vTrace("  WITH_BUCKET_HASHING\n"_Stz);
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
  
#ifdef WITH_ENCODING_TRACE
  vTrace("  WITH_ENCODING_TRACE\n"_Stz);
#endif
  
#ifdef WITH_ENTRY_EXIT
  vTrace("  WITH_ENTRY_EXIT\n"_Stz);
#endif
  
#ifdef WITH_GENVAR2
  vTrace("  WITH_GENVAR2\n"_Stz);
#endif
  
#ifdef WITH_INFLATE_TRACE
  vTrace("  WITH_INFLATE_TRACE\n"_Stz);
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
  
#ifdef WITH_PULSING
  vTrace("  WITH_PULSING\n"_Stz);
#endif
  
#ifdef WITH_RELEASE_VERSION
  vTrace("  WITH_RELEASE_VERSION\n"_Stz);
#endif
  
#ifdef WITH_SHARED_LOG
  vTrace("  WITH_SHARED_LOG\n"_Stz);
#endif
  
#ifdef WITH_SHOCO_DUPLICATES
  vTrace("  WITH_SHOCO_DUPLICATES\n"_Stz);
#endif
  
#ifdef WITH_STATISTICS_COUNTERS
  vTrace("  WITH_STATISTICS_COUNTERS\n"_Stz);
#endif
  
#ifdef WITH_TRACE_API
  vTrace("  WITH_TRACE_API\n"_Stz);
#endif
  
#ifdef WITH_TRACE_DYNAMO
  vTrace("  WITH_TRACE_DYNAMO\n"_Stz);
#endif
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
