/*============================================================================*/
/* Debug.hpp -- Debug Macro Definitions and Functions                         */
/*============================================================================*/
#pragma once

namespace Debugging
{
  static const i8 C_STACK_BACKTRACE = 10;

  void vAssertCalled(void);
  void vAssertDebug(void);
  void vAssertRelease(void);
  void vAssertTrap(void);
  void vAssertType(void* aObject, eCN eExpectingClass);
  void vAssertWarn(void);
  void vDebugPoint(void);
  void vDebugTrap(void);
  void vStackDump(void);
  void vThrowTrap(void);
  
  /*--------------------------------------------------------------------------*/
  /* check the object is within the pool                                      */
  /*--------------------------------------------------------------------------*/
  #ifdef WITH_ADDRESS_CHECKING
    #define _poolAddress_(pArg) Base::vCheckAddress(pArg)
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
        /* Debugging::vAssertDebug(); */ \
        Debugging::vAssertTrap(); \
        THROW_ERROR_MSG(EC_ASSERT,(__builtin_expect(!(condition), 0) \
                                    ? (Stz*) #condition : ""_Stz)); \
    } \
  }
  #else
    #define ASSERT(condition)
  #endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define BEEN_CALLED(_condition) \
  { \
    if ( !(_condition) ) { \
      printf("BEEN_CALLED in %s at %d\n",__FILE__,__LINE__); \
    Debugging::vAssertCalled(); \
    } \
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT(condition) \
  { \
    if ( !(condition) ) { \
      printf("RASSERT in %s at %d\n",__FILE__,__LINE__); \
      Debugging::vAssertRelease(); \
    } \
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT_TYPE(pARG,eCLASS) \
  { \
    if ( pARG && eCLASS != pARG->eClass()  ) { \
      printf("RASSERT_TYPE in %s at %d\n",__FILE__,__LINE__); \
      Debugging::vAssertType(pARG,eCLASS); \
    } \
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  #define RASSERT_WARN(condition) \
  { \
    if ( !(condition) ) { \
      printf("RASSERT_WARN in %s at %d\n",__FILE__,__LINE__); \
      Debugging::vAssertWarn(); \
    } \
  }
        
  /*--------------------------------------------------------------------------*/
  /* NEVER_REACH macro (point in code which should never be reached           */
  /*--------------------------------------------------------------------------*/
  #define NEVER_REACH { \
    Debugging::vAssertTrap(); \
    THROW_ERROR_MSG(EC_ASSERT,"NEVER_REACH"_Stz); \
  }
  
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
