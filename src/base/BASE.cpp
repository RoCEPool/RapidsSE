/*============================================================================*/
/* Base.cpp -- Base namespace                                                 */
/*============================================================================*/

#include "BASE.h"

#include "TagConstants.h"

SubSysFn __Base_SubSystem("Base"_Stz,(fpSELFFN*)BASE::vSubSystem,
                          ""_Stz,
                          "Files"_Stz,nullptr);

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void BASE::vCheckAssumptions(void)
{
  // resources
  
  ALWAYS(4 == sizeof(u4));
  ALWAYS(8 == sizeof(u8));
  ALWAYS(8 == sizeof(i8));

  ALWAYS(sizeof(struct sockaddr_in) <= (8 * 16));
  
  // pool
  
  //ASSERT(sizeof(Object) + sizeof(bn)*2 + sizeof(void*) == sizeof(PoolBuffer));
  //ASSERT(sizeof(PoolBuffer) + sizeof(void*)*2          == sizeof(HeapBuffer));
  
  ALWAYS(sizeof(HeapBuffer) == 1 * sizeof(CacheLine));
  
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_TINY_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_SMALL_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_MEDIUM_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_LARGE_OBJECT));

  ASSERT(CacheLine::bMultipleCacheLines(sizeof(LinkedBuffer)));

  PoolHelper::vValidateEnums();
  
  
  // shared memory
  
  
  // alignment
 
  ALWAYS(sizeof(SingleWord) == 8);
  ALWAYS(sizeof(SingleWord) == C_BYTES_PER_WORD);
  ALWAYS(sizeof(DoubleWord) == 2 * C_BYTES_PER_WORD);
  ALWAYS(sizeof(QuadWord)   == 4 * C_BYTES_PER_WORD);
  ALWAYS(sizeof(CacheLine)  == C_BYTES_PER_CACHE_LINE);
  
  // locking
  
  
  // host
  
  ALWAYS(Process::bTestPid(Process::iGetPid()));

  
  // library
  
  ALWAYS(8 == sizeof(Dtm));

  
  // focals
  
  
  // enums
  
  
  // esses
  
  
  // correctness
  
  ALWAYS(sizeof(u2)  == sizeof(eCN));
    
  ALWAYS(8 == sizeof(Header));
  ALWAYS(8 == sizeof(Object));
  ALWAYS(16 == sizeof(Footer));
  

  ALWAYS(8 == sizeof(Id_2part));
  
  ALWAYS(8 == sizeof(Bcd));
  ALWAYS(8 == sizeof(Bol));
  ALWAYS(8 == sizeof(Dte));
  ALWAYS(8 == sizeof(Edt));
  ALWAYS(8 == sizeof(Flt));
  ALWAYS(8 == sizeof(Int));
  ALWAYS(8 == sizeof(Ivl));
  ALWAYS(8 == sizeof(Tme));
  ALWAYS(8 == sizeof(Tsp));
  
  
  // class data
  
  
  // strings
  
  
  // assembler
  
  
  // base
  
  ALWAYS(8 == sizeof(ColPix));

}

/*----------------------------------------------------------------------------*/
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void BASE::vSubSystem(TokenVec* pTokenVec)
{
  _static_
  UNUSED(pTokenVec);

  vTrace("BASE::vSubSystem\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/* test with                                                                  */
/*----------------------------------------------------------------------------*/
bool BASE::bTraceWith(Stz* szWith)
{
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
    
#ifdef WITH_TRACE_ENCODING
  if (szWith->bSame("WITH_TRACE_ENCODING"_Stz))
    return true;
#endif
  
#ifdef WITH_STEP_TIMING
  if (szWith->bSame("WITH_STEP_TIMING"_Stz))
    return true;
#endif
    
#ifdef WITH_TRACE_INFLATE
  if (szWith->bSame("WITH_TRACE_INFLATE"_Stz))
    return true;
#endif
  
#ifdef WITH_TRACE_INDEX_COMPACTION
  if (szWith->bSame("WITH_TRACE_INDEX_COMPACTION"_Stz))
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
    
#ifdef WITH_PYTHON_SUPPORT
  if (szWith->bSame("WITH_PYTHON_SUPPORT"_Stz))
    return true;
#endif
  
#ifdef WITH_RELEASE_VERSION
  if (szWith->bSame("WITH_RELEASE_VERSION"_Stz))
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
  
      
  return false;
}

/*----------------------------------------------------------------------------*/
/* trace with                                                                 */
/*----------------------------------------------------------------------------*/
void BASE::vTraceWith(void)
{
  vTrace("\nWith:\n"_Stz);
    
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
  
#ifdef WITH_TRACE_ENCODING
  vTrace("  WITH_TRACE_ENCODING\n"_Stz);
#endif
  
#ifdef WITH_STEP_TIMING
  vTrace("  WITH_STEP_TIMING\n"_Stz);
#endif
    
#ifdef WITH_TRACE_INFLATE
  vTrace("  WITH_TRACE_INFLATE\n"_Stz);
#endif
  
#ifdef WITH_TRACE_INDEX_COMPACTION
  vTrace("  WITH_TRACE_INDEX_COMPACTION\n"_Stz);
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
    
#ifdef WITH_PYTHON_SUPPORT
  vTrace("  WITH_PYTHON_SUPPORT\n"_Stz);
#endif
  
#ifdef WITH_RELEASE_VERSION
  vTrace("  WITH_RELEASE_VERSION\n"_Stz);
#endif
      
#ifdef WITH_STATISTICS_COUNTERS
  vTrace("  WITH_STATISTICS_COUNTERS\n"_Stz);
#endif
  
#ifdef WITH_TRACE_API
  vTrace("  WITH_TRACE_API\n"_Stz);
#endif
      
}

/*----------------------------------------------------------------------------*/
/* test without                                                                  */
/*----------------------------------------------------------------------------*/
bool BASE::bTraceWithout(Stz* szWith)
{
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
  
#ifndef WITH_TRACE_ENCODING
  if (szWith->bSame("WITH_TRACE_ENCODING"_Stz))
    return true;
#endif
  
#ifndef WITH_STEP_TIMING
  if (szWith->bSame("WITH_STEP_TIMING"_Stz))
    return true;
#endif
    
#ifndef WITH_TRACE_INFLATE
  if (szWith->bSame("WITH_TRACE_INFLATE"_Stz))
    return true;
#endif
  
#ifndef WITH_TRACE_INDEX_COMPACTION
  if (szWith->bSame("WITH_TRACE_INDEX_COMPACTION"_Stz))
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
    
#ifndef WITH_PYTHON_SUPPORT
  if (szWith->bSame("WITH_PYTHON_SUPPORT"_Stz))
    return true;
#endif
  
#ifndef WITH_RELEASE_VERSION
  if (szWith->bSame("WITH_RELEASE_VERSION"_Stz))
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
      
  return false;
}

/*----------------------------------------------------------------------------*/
/* trace what hasn't been compiled with                                       */
/*----------------------------------------------------------------------------*/
void BASE::vTraceWithout(void)
{
  vTrace("\nWithout:\n"_Stz);
    
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
    
#ifndef WITH_TRACE_ENCODING
  vTrace("  WITH_TRACE_ENCODING\n"_Stz);
#endif
  
#ifndef WITH_STEP_TIMING
  vTrace("  WITH_STEP_TIMING\n"_Stz);
#endif
    
#ifndef WITH_TRACE_INFLATE
  vTrace("  WITH_TRACE_INFLATE\n"_Stz);
#endif
  
#ifndef WITH_TRACE_INDEX_COMPACTION
  vTrace("  WITH_TRACE_INDEX_COMPACTION\n"_Stz);
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
    
#ifndef WITH_PYTHON_SUPPORT
  vTrace("  WITH_PYTHON_SUPPORT\n"_Stz);
#endif
  
#ifndef WITH_RELEASE_VERSION
  vTrace("  WITH_RELEASE_VERSION\n"_Stz);
#endif
    
#ifndef WITH_STATISTICS_COUNTERS
  vTrace("  WITH_STATISTICS_COUNTERS\n"_Stz);
#endif
  
#ifndef WITH_TRACE_API
  vTrace("  WITH_TRACE_API\n"_Stz);
#endif
       
#ifndef WITH_TRACE_LOGICUNIT
  vTrace("  WITH_TRACE_LOGICUNIT\n"_Stz);
#endif
  
}

/*----------------------------------------------------------------------------*/
/* initialise all globals                                                     */
/*----------------------------------------------------------------------------*/
void BASE::vInitAllGlobals(void)
{
  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("BASE::vInitAllGlobals\n"_Stz);
  
  File::vInitGlobals();
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
