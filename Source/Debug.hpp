/*============================================================================*/
/* Debug.hpp -- Debug Macro Definitions and Functions                         */
/*============================================================================*/
#pragma once

namespace Debugging
{
  static const i8 C_STACK_BACKTRACE = 10;

  void vAssertTrap(void);
  void vDebugTrap(void);
  void vStackDump(void);
  void vThrowTrap(void);
  
  /*--------------------------------------------------------------------------*/
  /* check the object is within the pool                                      */
  /*--------------------------------------------------------------------------*/
  #ifdef __ADDRESS_CHECK
    #define _poolAddress_(pArg) Base::vCheckAddress(pArg)
  #else
    #define _poolAddress_(pArg)
  #endif
  
  /*--------------------------------------------------------------------------*/
  /* our ASSERT macro                                                         */
  /*--------------------------------------------------------------------------*/
  #ifdef __ASSERT_CHECKS
    #define ASSERT(condition) \
    { \
      if ( !(condition) ) { \
        printf("ASSERT in %s at %d\n",__FILE__,__LINE__); \
        Debugging::vAssertTrap(); \
        THROW_ERROR_MSG(EC_ASSERT,(__builtin_expect(!(condition), 0) \
                                    ? (Stz*) #condition : ""_Stz)); \
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
      Debugging::vAssertTrap(); \
      THROW_ERROR_MSG(EC_ASSERT,(__builtin_expect(!(condition), 0) \
                                  ? (Stz*) #condition : ""_Stz)); \
    } \
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT_TYPE(pARG,eCLASS) \
  { \
    if ( pARG && eCLASS != pARG->eClass()  ) { \
      printf("RASSERT_TYPE in %s at %d\n",__FILE__,__LINE__); \
      printf("  Expecting class %d; Got %d\n",eCLASS,pARG->eClass()); \
      Debugging::vAssertTrap(); \
      THROW_ERROR_MSG(EC_ASSERT, \
                      (__builtin_expect(!(pARG && eCLASS != pARG->eClass()), 0) \
                                  ? (Stz*) #eCLASS : ""_Stz)); \
    } \
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT_WARN(condition) \
  { \
    if ( !(condition) ) { \
      printf("RWARN in %s at %d\n",__FILE__,__LINE__); \
      Debugging::vAssertTrap(); \
    } \
  }
  
  /*--------------------------------------------------------------------------*/
  /* initialise memory to zero                                                */
  /*--------------------------------------------------------------------------*/
  #ifdef __MEMORY_ZERO
    #define _memzero_(pAddr,nBytes)  MemLib::vZero(pAddr,nBytes);
  #else
    #define _memzero_(pAddr,nBytes)
  #endif
  
  /*--------------------------------------------------------------------------*/
  /* check the object is within the pool                                      */
  /*--------------------------------------------------------------------------*/
  #ifdef MP_CHECK
    #define _mp_(pArg) ASSERT(pArg)
  #else
    #define _mp_(pArg)
  #endif
    
  /*--------------------------------------------------------------------------*/
  /* NEVER_REACH macro (point in code which should never be reached           */
  /*--------------------------------------------------------------------------*/
  #define NEVER_REACH \
    Debugging::vAssertTrap(); \
    THROW_ERROR_MSG(EC_ASSERT,"CodePath"_Stz);

  /*--------------------------------------------------------------------------*/
  /* throw exception macro's                                                  */
  /*--------------------------------------------------------------------------*/
  
  #define THROW_ERROR(exception) \
    throw new Error(exception,(Stz*)__FILE__,__LINE__);
  
  #define THROW_ERROR_MSG(exception, msg) \
    throw new Error(exception,msg,(Stz*)__FILE__,__LINE__);

};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
