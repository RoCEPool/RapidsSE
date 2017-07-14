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
        Debugging::vAssertTrap(); \
        THROW_ERROR_MSG(EC_ASSERT,(__builtin_expect(!(condition), 0) \
                                    ? (Str*) #condition : ""_Str)); \
    } \
  }
  #else
    #define ASSERT(condition)
  #endif
    
  #define RASSERT(condition) \
  { \
    if ( !(condition) ) { \
      Debugging::vAssertTrap(); \
      THROW_ERROR_MSG(EC_ASSERT,(__builtin_expect(!(condition), 0) \
                                  ? (Str*) #condition : ""_Str)); \
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
    THROW_ERROR_MSG(EC_ASSERT,"CodePath"_Str);

  /*--------------------------------------------------------------------------*/
  /* throw exception macro's                                                  */
  /*--------------------------------------------------------------------------*/
  
  #define THROW_ERROR(exception) \
    throw new Error(exception,(Str*)__FILE__,__LINE__);
  
  #define THROW_ERROR_MSG(exception, msg) \
    throw new Error(exception,msg,(Str*)__FILE__,__LINE__);

};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
