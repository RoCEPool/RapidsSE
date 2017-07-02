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
  #ifdef ADDRESS_CHECK
    #define _poolAddress_(pArg) Base::vCheckAddress(pArg)
  #else
    #define _poolAddress_(pArg)
  #endif
  
  /*--------------------------------------------------------------------------*/
  /* our ASSERT macro                                                         */
  /*--------------------------------------------------------------------------*/
  #ifdef ASSERT_CHECKS
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
  /* check the object is of a known class                                     */
  /*--------------------------------------------------------------------------*/
  #ifdef CLASS_CHECK
    #define _class_(arg) ASSERT(eClass() == arg);
    #define _class2_(arg1,arg2) ASSERT(eClass() == arg1 || eClass() == arg2);
    #define _class3_(arg1,arg2,arg3) \
    ASSERT(eClass() == arg1 || eClass() == arg2 || eClass() == arg3);
  #else
    #define _class_(arg)
    #define _class2_(arg1,arg2)
    #define _class3_(arg1,arg2,arg3)
  #endif
  
  /*--------------------------------------------------------------------------*/
  /* check the object has a footer marker as its last word                    */
  /*--------------------------------------------------------------------------*/
  #ifdef FOOTER_CHECK
    #define _footer_() aFooter()->vAssertFooter();
    #define _hazard_() m_xHazard.vAssertFooter();
    #define _setFooter_() aFooter()->vSetFooter();
  #else
    #define _footer_()
    #define _hazard_()
    #define _setFooter_()
  #endif
  
  /*--------------------------------------------------------------------------*/
  /* check the object has a magic marker as its first word                    */
  /*--------------------------------------------------------------------------*/
  #ifdef MAGIC_CHECK
    #define _magic_() ASSERT(iMagic() == MAGIC);
  #else
    #define _magic_()
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
  /* pre, post condition and check initialised                                */
  /*--------------------------------------------------------------------------*/
  #define _ADDRESS_ 1
  #define _MAGIC_   2
  #define _FOOTER_  3
  
  #define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
  
  #define _precondition_(...) \
    GET_MACRO(__VA_ARGS__, \
              _precondition4_, _precondition3_, \
              _precondition2_, _precondition1_)(__VA_ARGS__)
  
  #define _postcondition_(...) \
    GET_MACRO(__VA_ARGS__, \
              _precondition4_, _precondition3_, \
              _precondition2_, _precondition1_)(__VA_ARGS__)
  
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
          _poolAddress_(this); \
        }                                    \
        else if (checkValue == _FOOTER_)     \
        {                                    \
          _footer_();                        \
        }                                    \
        else {                               \
          ASSERT("Should never be reached"_Str); \
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
    
    // check initialized
    #define _checkInitialised_(eClass) \
      MemLib::vCheckInitialised(this,eClass);
    
    // check initialized
    #define _checkInitialisedSize_(iSize) \
      MemLib::vCheckInitialised(this,iSize);

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
    
    // check initialized
    #define _checkInitialised_(eClass)
    #define _checkInitialisedSize_(iSize)

  #endif
  
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
