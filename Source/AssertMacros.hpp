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
      if (! (condition)) \
        Correctness::vAssert(__FILE__,__LINE__,#condition);
  #else
    #define ASSERT(condition)
  #endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT(condition) \
    if (! (condition)) \
      Correctness::vAssert(__FILE__,__LINE__,#condition);

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
        

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
