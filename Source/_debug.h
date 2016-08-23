/*============================================================================*/
/* _debug.h -- Debug Macro Definitions                                        */
/*============================================================================*/

#pragma once

namespace Common
{
  void vAssertTrap(void);
  void vDebugTrap(void);
  
  #ifdef MAGIC_CHECK
  #define _magic_() ASSERT(iMagic() == MAGIC);
  #else
  #define _magic_()
  #endif
    
  #ifdef CLASS_CHECK
  #define _class_(arg) ASSERT(eClass() == arg);
  #else
  #define _class_(arg)
  #endif
  
  #ifdef FOOTER_CHECK
  #define _footer_() pFooter()->vAssertFooter();
  #else
  #define _footer_(arg)
  #endif
  
  #ifdef ADDRESS_CHECK
  #define _poolAddress_(pArg) gpBufferPool->vCheckAddress(pArg)
  #else
  #define _poolAddress_(pArg)
  #endif

  // ASSERT
  #define ASSERT(condition) \
  { \
    if ( !(condition) ) { \
      Common::vAssertTrap(); \
      THROW_ERROR(EC_ASSERT); \
    } \
  }
  
  // precondition for any method is a valid this pointer and valid magic
  
  #define _ADDRESS_ true
  #define _MAGIC_ true
  #define _FOOTER_ true
  
  #define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
  #define _precondition_(...) GET_MACRO(__VA_ARGS__, _precondition4_, _precondition3_, _precondition2_)(__VA_ARGS__)
  
  #ifdef DEBUG_CHECKS
  #define _precondition2_(address,magic) \
    if (address) \
      gpBufferPool->vCheckAddress(this); \
    if (magic) \
      _magic_();
  
  #define _precondition3_(address, magic, eClass) \
    if (address) \
      gpBufferPool->vCheckAddress(this); \
    if (magic) \
      _magic_(); \
    if (eClass != CN_NONE) \
      _class_(eClass); \

  #define _precondition4_(address, magic, eClass, foot) \
    if (address) \
      gpBufferPool->vCheckAddress(this); \
    if (magic) \
      _magic_(); \
    if (eClass != CN_NONE) \
      _class_(eClass); \
    if (foot) \
      _footer_();
  
  #else
  
  #define _precondition2_(address, magic)
  #define _precondition3_(address, magic, eClass)
  #define _precondition4_(address, magic, eClass, pFooter)
  
  #endif
  
  // throw
  #define THROW_ERROR(exception) \
    throw new Error(exception,__FILE__,__LINE__);
  
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/