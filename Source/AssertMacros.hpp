/*============================================================================*/
/* AssertMacros.hpp -- Assert Macro Definitions                               */
/*============================================================================*/
#pragma once


  /*--------------------------------------------------------------------------*/
  /* check the object is within the pool                                      */
  /*--------------------------------------------------------------------------*/
  #ifdef WITH_ADDRESS_CHECKING
    #define _poolAddress_(pArg) Correctness::vCheckAddress(pArg)
  #else
    #define _poolAddress_(pArg)
  #endif
  
  /*--------------------------------------------------------------------------*/
  /* our ASSERT macros                                                        */
  /*--------------------------------------------------------------------------*/
  #ifdef WITH_ASSERT_MACRO
    #define ASSERT(condition) \
    { \
      if ( !(condition) ) { \
        printf("ASSERT in %s at %d\n",__FILE__,__LINE__); \
        Correctness::vAsserted((const char*)#condition,(const char*)__FILE__,__LINE__); \
        Correctness::vAssertPoint(); \
        Correctness::vAssertTrap(); \
    } \
  }
  #else
    #define ASSERT(condition)
  #endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT(condition) \
  { \
    if ( !(condition) ) { \
      printf("RASSERT in %s at %d\n",__FILE__,__LINE__); \
      Correctness::vAsserted((const char*)#condition,(const char*)__FILE__,__LINE__); \
      Correctness::vAssertPoint(); \
      Correctness::vAssertRelease(); \
    } \
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT_TYPE(pARG,eCLASS) \
  { \
    if ( pARG && eCLASS != pARG->eClass()  ) { \
      printf("RASSERT_TYPE in %s at %d\n",__FILE__,__LINE__); \
      Correctness::vAssertType(pARG,eCLASS); \
    } \
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT_WARN(condition) \
  { \
    if ( !(condition) ) { \
      printf("RASSERT_WARN in %s at %d\n",__FILE__,__LINE__); \
      Correctness::vAssertWarn(); \
    } \
  }
        
  /*--------------------------------------------------------------------------*/
  /* NEVER_REACH macro (point in code which should never be reached           */
  /*--------------------------------------------------------------------------*/
  #define NEVER_REACH { \
    Correctness::vAssertPoint(); \
    Correctness::vAssertTrap(); \
    throw(-1); \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
