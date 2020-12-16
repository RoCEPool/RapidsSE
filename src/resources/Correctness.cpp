/*============================================================================*/
/* Correctness.cpp -- Correctness Algebra namespace functions                 */
/*============================================================================*/

#include "Correctness.h"

/*----------------------------------------------------------------------------*/
/* test with                                                                  */
/*----------------------------------------------------------------------------*/
bool Resources::bTraceWith(Stz* szWith)
{
#ifdef WITH_ACTION_SEQUENCE
  if (szWith->bSame("WITH_ACTION_SEQUENCE"_Stz))
    return true;
#endif
  
#ifdef WITH_ADDRESS_CHECKING
  if (szWith->bSame("WITH_ADDRESS_CHECKING"_Stz))
    return true;
#endif
  
#ifdef WITH_APPLE
  if (szWith->bSame("WITH_APPLE"_Stz))
    return true;
#endif
  
#ifdef WITH_ANALYSE_FREE
  if (szWith->bSame("WITH_ANALYSE_FREE"_Stz))
    return true;
#endif
  
#ifdef WITH_ASSERT_MACRO
  if (szWith->bSame("WITH_ASSERT_MACRO"_Stz))
    return true;
#endif
  
#ifdef WITH_BUCKET_HASHING
  if (szWith->bSame("WITH_BUCKET_HASHING"_Stz))
    return true;
#endif
  
#ifdef WITH_CHECK_INITIALISED
  if (szWith->bSame("WITH_CHECK_INITIALISED"_Stz))
    return true;
#endif
  
#ifdef WITH_CORRECTNESS_ALGEBRA
  if (szWith->bSame("WITH_CORRECTNESS_ALGEBRA"_Stz))
    return true;
#endif
      
#ifdef WITH_DEBUG_VERSION
  if (szWith->bSame("WITH_DEBUG_VERSION"_Stz))
    return true;
#endif
  
#ifdef WITH_EB_LAZY_CHAIN
  if (szWith->bSame("WITH_EB_LAZY_CHAIN"_Stz))
    return true;
#endif
  
#ifdef WITH_ENCODING_TRACE
  if (szWith->bSame("WITH_ENCODING_TRACE"_Stz))
    return true;
#endif
  
#ifdef WITH_ENTRY_EXIT
  if (szWith->bSame("WITH_ENTRY_EXIT"_Stz))
    return true;
#endif
    
#ifdef WITH_INFLATE_TRACE
  if (szWith->bSame("WITH_INFLATE_TRACE"_Stz))
    return true;
#endif
  
#ifdef WITH_IT_COMPACT_TRACE
  if (szWith->bSame("WITH_IT_COMPACT_TRACE"_Stz))
    return true;
#endif
  
#ifdef WITH_LINUX
  if (szWith->bSame("WITH_LINUX"_Stz))
    return true;
#endif
  
#ifdef WITH_LOCK_TRACKING
  if (szWith->bSame("WITH_LOCK_TRACKING"_Stz))
    return true;
#endif
  
#ifdef WITH_MUNGED_SEN
  if (szWith->bSame("WITH_MUNGED_SEN"_Stz))
    return true;
#endif
  
#ifdef WITH_PREPOST_CHECKING
  if (szWith->bSame("WITH_PREPOST_CHECKING"_Stz))
    return true;
#endif
  
#ifdef WITH_PULSING
  if (szWith->bSame("WITH_PULSING"_Stz))
    return true;
#endif
  
#ifdef WITH_PYTHON_SUPPORT
  if (szWith->bSame("WITH_PYTHON_SUPPORT"_Stz))
    return true;
#endif
  
#ifdef WITH_RELEASE_VERSION
  if (szWith->bSame("WITH_RELEASE_VERSION"_Stz))
    return true;
#endif
    
#ifdef WITH_SETH_SUPPORT
  if (szWith->bSame("WITH_SETH_SUPPORT"_Stz))
    return true;
#endif
  
#ifdef WITH_SHARED_LOG
  if (szWith->bSame("WITH_SHARED_LOG"_Stz))
    return true;
#endif
  
#ifdef WITH_SHOCO_DUPLICATES
  if (szWith->bSame("WITH_SHOCO_DUPLICATES"_Stz))
    return true;
#endif
  
#ifdef WITH_STATISTICS_COUNTERS
  if (szWith->bSame("WITH_STATISTICS_COUNTERS"_Stz))
    return true;
#endif
  
#ifdef WITH_TRACE_API
  if (szWith->bSame("WITH_TRACE_API"_Stz))
    return true;
#endif
  
#ifdef WITH_TRACE_DYNAMO
  if (szWith->bSame("WITH_TRACE_DYNAMO"_Stz))
    return true;
#endif
  
#ifdef WITH_TRACE_PURGING
  if (szWith->bSame("WITH_TRACE_PURGING"_Stz))
    return true;
#endif
    
  return false;
}

/*----------------------------------------------------------------------------*/
/* trace with                                                                 */
/*----------------------------------------------------------------------------*/
void Resources::vTraceWith(void)
{
  vTrace("\nWith:\n"_Stz);

#ifdef WITH_ACTION_SEQUENCE
  vTrace("  WITH_ACTION_SEQUENCE\n"_Stz);
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
  
#ifdef WITH_CORRECTNESS_ALGEBRA
  vTrace("  WITH_CORRECTNESS_ALGEBRA\n"_Stz);
#endif
      
#ifdef WITH_DEBUG_VERSION
  vTrace("  WITH_DEBUG_VERSION\n"_Stz);
#endif
  
#ifdef WITH_EB_LAZY_CHAIN
  vTrace("  WITH_EB_LAZY_CHAIN\n"_Stz);
#endif
  
#ifdef WITH_ENCODING_TRACE
  vTrace("  WITH_ENCODING_TRACE\n"_Stz);
#endif
  
#ifdef WITH_ENTRY_EXIT
  vTrace("  WITH_ENTRY_EXIT\n"_Stz);
#endif
    
#ifdef WITH_INFLATE_TRACE
  vTrace("  WITH_INFLATE_TRACE\n"_Stz);
#endif
  
#ifdef WITH_IT_COMPACT_TRACE
  vTrace("  WITH_IT_COMPACT_TRACE\n"_Stz);
#endif
  
#ifdef WITH_LINUX
  vTrace("  WITH_LINUX\n"_Stz);
#endif
  
#ifdef WITH_LOCK_TRACKING
  vTrace("  WITH_LOCK_TRACKING\n"_Stz);
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
  
#ifdef WITH_PYTHON_SUPPORT
  vTrace("  WITH_PYTHON_SUPPORT\n"_Stz);
#endif
  
#ifdef WITH_RELEASE_VERSION
  vTrace("  WITH_RELEASE_VERSION\n"_Stz);
#endif
    
#ifdef WITH_SETH_SUPPORT
  vTrace("  WITH_SETH_SUPPORT\n"_Stz);
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
  
#ifdef WITH_TRACE_PURGING
  vTrace("  WITH_TRACE_PURGING\n"_Stz);
#endif
  
}

/*----------------------------------------------------------------------------*/
/* test without                                                                  */
/*----------------------------------------------------------------------------*/
bool Resources::bTraceWithout(Stz* szWith)
{
#ifndef WITH_ACTION_SEQUENCE
  if (szWith->bSame("WITH_ACTION_SEQUENCE"_Stz))
    return true;
#endif
  
#ifndef WITH_ADDRESS_CHECKING
  if (szWith->bSame("WITH_ADDRESS_CHECKING"_Stz))
    return true;
#endif
  
#ifndef WITH_APPLE
  if (szWith->bSame("WITH_APPLE"_Stz))
    return true;
#endif
  
#ifndef WITH_ANALYSE_FREE
  if (szWith->bSame("WITH_ANALYSE_FREE"_Stz))
    return true;
#endif
  
#ifndef WITH_ASSERT_MACRO
  if (szWith->bSame("WITH_ASSERT_MACRO"_Stz))
    return true;
#endif
  
#ifndef WITH_BUCKET_HASHING
  if (szWith->bSame("WITH_BUCKET_HASHING"_Stz))
    return true;
#endif
  
#ifndef WITH_CHECK_INITIALISED
  if (szWith->bSame("WITH_CHECK_INITIALISED"_Stz))
    return true;
#endif
  
#ifndef WITH_CORRECTNESS_ALGEBRA
  if (szWith->bSame("WITH_CORRECTNESS_ALGEBRA"_Stz))
    return true;
#endif
      
#ifndef WITH_DEBUG_VERSION
  if (szWith->bSame("WITH_DEBUG_VERSION"_Stz))
    return true;
#endif
  
#ifndef WITH_EB_LAZY_CHAIN
  if (szWith->bSame("WITH_EB_LAZY_CHAIN"_Stz))
    return true;
#endif
  
#ifndef WITH_ENCODING_TRACE
  if (szWith->bSame("WITH_ENCODING_TRACE"_Stz))
    return true;
#endif
  
#ifndef WITH_ENTRY_EXIT
  if (szWith->bSame("WITH_ENTRY_EXIT"_Stz))
    return true;
#endif
    
#ifndef WITH_INFLATE_TRACE
  if (szWith->bSame("WITH_INFLATE_TRACE"_Stz))
    return true;
#endif
  
#ifndef WITH_IT_COMPACT_TRACE
  if (szWith->bSame("WITH_IT_COMPACT_TRACE"_Stz))
    return true;
#endif
  
#ifndef WITH_LINUX
  if (szWith->bSame("WITH_LINUX"_Stz))
    return true;
#endif
  
#ifndef WITH_LOCK_TRACKING
  if (szWith->bSame("WITH_LOCK_TRACKING"_Stz))
    return true;
#endif
  
#ifndef WITH_MUNGED_SEN
  if (szWith->bSame("WITH_MUNGED_SEN"_Stz))
    return true;
#endif
  
#ifndef WITH_PREPOST_CHECKING
  if (szWith->bSame("WITH_PREPOST_CHECKING"_Stz))
    return true;
#endif
  
#ifndef WITH_PULSING
  if (szWith->bSame("WITH_PULSING"_Stz))
    return true;
#endif
  
#ifndef WITH_PYTHON_SUPPORT
  if (szWith->bSame("WITH_PYTHON_SUPPORT"_Stz))
    return true;
#endif
  
#ifndef WITH_RELEASE_VERSION
  if (szWith->bSame("WITH_RELEASE_VERSION"_Stz))
    return true;
#endif
    
#ifndef WITH_SETH_SUPPORT
  if (szWith->bSame("WITH_SETH_SUPPORT"_Stz))
    return true;
#endif
  
#ifndef WITH_SHARED_LOG
  if (szWith->bSame("WITH_SHARED_LOG"_Stz))
    return true;
#endif
  
#ifndef WITH_SHOCO_DUPLICATES
  if (szWith->bSame("WITH_SHOCO_DUPLICATES"_Stz))
    return true;
#endif
  
#ifndef WITH_STATISTICS_COUNTERS
  if (szWith->bSame("WITH_STATISTICS_COUNTERS"_Stz))
    return true;
#endif
  
#ifndef WITH_TRACE_API
  if (szWith->bSame("WITH_TRACE_API"_Stz))
    return true;
#endif
  
#ifndef WITH_TRACE_DYNAMO
  if (szWith->bSame("WITH_TRACE_DYNAMO"_Stz))
    return true;
#endif
  
#ifndef WITH_TRACE_PURGING
  if (szWith->bSame("WITH_TRACE_PURGING"_Stz))
    return true;
#endif
    
  return false;
}

/*----------------------------------------------------------------------------*/
/* trace what hasn't been compiled with                                       */
/*----------------------------------------------------------------------------*/
void Resources::vTraceWithout(void)
{
  vTrace("\nWithout:\n"_Stz);

#ifndef WITH_ACTION_SEQUENCE
  vTrace("  WITH_ACTION_SEQUENCE\n"_Stz);
#endif
  
#ifndef WITH_ADDRESS_CHECKING
  vTrace("  WITH_ADDRESS_CHECKING\n"_Stz);
#endif
  
#ifndef WITH_APPLE
  vTrace("  WITH_APPLE\n"_Stz);
#endif
  
#ifndef WITH_ANALYSE_FREE
  vTrace("  WITH_ANALYSE_FREE\n"_Stz);
#endif
  
#ifndef WITH_ASSERT_MACRO
  vTrace("  WITH_ASSERT_MACRO\n"_Stz);
#endif
  
#ifndef WITH_BUCKET_HASHING
  vTrace("  WITH_BUCKET_HASHING\n"_Stz);
#endif
  
#ifndef WITH_CHECK_INITIALISED
  vTrace("  WITH_CHECK_INITIALISED\n"_Stz);
#endif
  
#ifndef WITH_CORRECTNESS_ALGEBRA
  vTrace("  WITH_CORRECTNESS_ALGEBRA\n"_Stz);
#endif
      
#ifndef WITH_DEBUG_VERSION
  vTrace("  WITH_DEBUG_VERSION\n"_Stz);
#endif
  
#ifndef WITH_EB_LAZY_CHAIN
  vTrace("  WITH_EB_LAZY_CHAIN\n"_Stz);
#endif
  
#ifndef WITH_ENCODING_TRACE
  vTrace("  WITH_ENCODING_TRACE\n"_Stz);
#endif
  
#ifndef WITH_ENTRY_EXIT
  vTrace("  WITH_ENTRY_EXIT\n"_Stz);
#endif
    
#ifndef WITH_INFLATE_TRACE
  vTrace("  WITH_INFLATE_TRACE\n"_Stz);
#endif
  
#ifndef WITH_IT_COMPACT_TRACE
  vTrace("  WITH_IT_COMPACT_TRACE\n"_Stz);
#endif
  
#ifndef WITH_LINUX
  vTrace("  WITH_LINUX\n"_Stz);
#endif
  
#ifndef WITH_LOCK_TRACKING
  vTrace("  WITH_LOCK_TRACKING\n"_Stz);
#endif
  
#ifndef WITH_MUNGED_SEN
  vTrace("  WITH_MUNGED_SEN\n"_Stz);
#endif
  
#ifndef WITH_PREPOST_CHECKING
  vTrace("  WITH_PREPOST_CHECKING\n"_Stz);
#endif
  
#ifndef WITH_PULSING
  vTrace("  WITH_PULSING\n"_Stz);
#endif
  
#ifndef WITH_PYTHON_SUPPORT
  vTrace("  WITH_PYTHON_SUPPORT\n"_Stz);
#endif
  
#ifndef WITH_RELEASE_VERSION
  vTrace("  WITH_RELEASE_VERSION\n"_Stz);
#endif
  
#ifndef WITH_SETH_SUPPORT
  vTrace("  WITH_SETH_SUPPORT\n"_Stz);
#endif
  
#ifndef WITH_SHARED_LOG
  vTrace("  WITH_SHARED_LOG\n"_Stz);
#endif
  
#ifndef WITH_SHOCO_DUPLICATES
  vTrace("  WITH_SHOCO_DUPLICATES\n"_Stz);
#endif
  
#ifndef WITH_STATISTICS_COUNTERS
  vTrace("  WITH_STATISTICS_COUNTERS\n"_Stz);
#endif
  
#ifndef WITH_TRACE_API
  vTrace("  WITH_TRACE_API\n"_Stz);
#endif
  
#ifndef WITH_TRACE_DYNAMO
  vTrace("  WITH_TRACE_DYNAMO\n"_Stz);
#endif
  
#ifndef WITH_TRACE_PROCEDURE
  vTrace("  WITH_TRACE_PROCEDURE\n"_Stz);
#endif
  
#ifndef WITH_TRACE_PURGING
  vTrace("  WITH_TRACE_PURGING\n"_Stz);
#endif
 
#ifndef WITH_TRACE_LOGICUNIT
  vTrace("  WITH_TRACE_LOGICUNIT\n"_Stz);
#endif
  
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
