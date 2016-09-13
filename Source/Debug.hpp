/*============================================================================*/
/* Debug.hpp -- Debug Macro Definitions and Functions                         */
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
  #define _footer_()
  #endif
  
  #ifdef ADDRESS_CHECK
  #define _poolAddress_(pArg) gpBufferPool->vCheckAddress(pArg)
  #else
  #define _poolAddress_(pArg)
  #endif
  
  #define ASSERT(condition) \
  { \
    if ( !(condition) ) { \
      Common::vAssertTrap(); \
      THROW_ERROR_MSG(EC_ASSERT,(__builtin_expect(!(condition), 0) ? #condition : "")); \
    } \
  }
  
  // precondition for any method is a valid this pointer, valid magic and a valid footer
  
  #define _ADDRESS_ 1
  #define _MAGIC_   2
  #define _FOOTER_  3
  
  #define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
  #define _precondition_(...) GET_MACRO(__VA_ARGS__, _precondition4_, _precondition3_, _precondition2_, _precondition1_)(__VA_ARGS__)
  #define _postcondition_(...) GET_MACRO(__VA_ARGS__, _precondition4_, _precondition3_, _precondition2_, _precondition1_)(__VA_ARGS__)
  
  #ifdef DEBUG_CHECKS
  
  #define _checkCondition_(checkValue)     \
    if (typeid(checkValue) == typeid(int)) \
    {                                      \
      if (checkValue == _MAGIC_)           \
      {                                    \
        _magic_();                         \
      }                                    \
      else if (checkValue == _ADDRESS_)    \
      {                                    \
        gpBufferPool->vCheckAddress(this); \
      }                                    \
      else if (checkValue == _FOOTER_)     \
      {                                    \
        _footer_();                        \
      }                                    \
      else {                               \
        ASSERT("Should never be reached"); \
      }                                    \
    }                                      \
    else {                                 \
      if (checkValue != CN_NONE)           \
      {                                    \
        _class_(checkValue);               \
      }                                    \
    }

//ASSERT(typeid(checkValue) == typeid(eCN));
  #define _precondition1_(checkValue1) \
    _checkCondition_(checkValue1);
  
  #define _precondition2_(checkValue1, checkValue2) \
    _checkCondition_(checkValue1); \
    _checkCondition_(checkValue2); 

  #define _precondition3_(checkValue1, checkValue2, checkValue3) \
    _checkCondition_(checkValue1); \
    _checkCondition_(checkValue2); \
    _checkCondition_(checkValue3);

  #define _precondition4_(checkValue1, checkValue2, checkValue3, checkValue4) \
    _checkCondition_(checkValue1); \
    _checkCondition_(checkValue2); \
    _checkCondition_(checkValue3); \
    _checkCondition_(checkValue4);

  #define _precondition5_(checkValue1, checkValue2, checkValue3, checkValue4, checkValue5) \
  _checkCondition_(checkValue1); \
  _checkCondition_(checkValue2); \
  _checkCondition_(checkValue3); \
  _checkCondition_(checkValue4); \
  _checkCondition_(checkValue5);
  
  #else
  
  #define _precondition1_(checkValue1)
  #define _precondition2_(checkValue1, checkValue2)
  #define _precondition3_(checkValue1, checkValue2, checkValue3)
  #define _precondition4_(checkValue1, checkValue2, checkValue3, checkValue4)
  
  #define _postcondition1_(checkValue1)
  #define _postcondition2_(checkValue1, checkValue2)
  #define _postcondition3_(checkValue1, checkValue2, checkValue3)
  #define _postcondition4_(checkValue1, checkValue2, checkValue3, checkValue4)
  
  #define _checkCondition_(checkValue)
  
  #endif
  
  // throw
  #define THROW_ERROR(exception) \
    throw new Error(exception,__FILE__,__LINE__);
  
  #define THROW_ERROR_MSG(exception, msg) \
    throw new Error(exception,msg,__FILE__,__LINE__);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/