/*============================================================================*/
/* Object.hpp -- Proper Object in system                                      */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class Object : public Header
  {
  private:
  
  public:
    Object(eCN eClass);
      
    Footer* aFooter(void);
  };
}

/*--------------------------------------------------------------------------*/
/* pre and post condition                                                   */
/*--------------------------------------------------------------------------*/
#define _ADDRESS_ 1 EndDefine
#define _MAGIC_   2 EndDefine
#define _FOOTER_  3 EndDefine

#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
EndDefine

#define _precondition_(...) \
  GET_MACRO(__VA_ARGS__, _precondition4_, _precondition3_, \
                         _precondition2_, _precondition1_)(__VA_ARGS__) 
EndDefine


#define _postcondition_(...) \
  GET_MACRO(__VA_ARGS__, _precondition4_, _precondition3_, \
                         _precondition2_, _precondition1_)(__VA_ARGS__) \
EndDefine


#ifdef WITH_PREPOST_CHECKING

  #define _checkCondition_(checkValue)       \
    if (typeid(checkValue) == typeid(int)) { \
      if (checkValue == _MAGIC_) {           \
        _magic_();                           \
      }                                      \
      else if (checkValue == _ADDRESS_)      \
      {                                      \
        _poolAddress_(this);                 \
      }                                      \
      else if (checkValue == _FOOTER_)       \
      {                                      \
        _footer_();                          \
      }                                      \
      else {                                   \
        ASSERT("Should never be reached"_Stz); \
      }                                        \
    }                                          \
    else {                                     \
      if (checkValue != CN_NONE)               \
      {                                        \
      _class_(checkValue);                     \
      }                                        \
    } \
  EndDefine

  //ASSERT(typeid(checkValue) == typeid(eCN));
  #define _precondition1_(checkValue1) \
    _checkCondition_(checkValue1); \
  EndDefine

  #define _precondition2_(checkValue1, checkValue2) \
    _checkCondition_(checkValue1); \
    _checkCondition_(checkValue2); \
  EndDefine

  #define _precondition3_(checkValue1, checkValue2, checkValue3) \
    _checkCondition_(checkValue1); \
    _checkCondition_(checkValue2); \
    _checkCondition_(checkValue3); \
  EndDefine

  #define _precondition4_(checkValue1, checkValue2, checkValue3, checkValue4) \
    _checkCondition_(checkValue1); \
    _checkCondition_(checkValue2); \
    _checkCondition_(checkValue3); \
    _checkCondition_(checkValue4); \
  EndDefine

#else

  #define _precondition1_(checkValue1) EndDefine
  #define _precondition2_(checkValue1, checkValue2) EndDefine
  #define _precondition3_(checkValue1, checkValue2, checkValue3) EndDefine
  #define _precondition4_(checkValue1, checkValue2, checkValue3, checkValue4) EndDefine

  #define _postcondition1_(checkValue1) EndDefine
  #define _postcondition2_(checkValue1, checkValue2) EndDefine
  #define _postcondition3_(checkValue1, checkValue2, checkValue3) EndDefine
  #define _postcondition4_(checkValue1, checkValue2, checkValue3, checkValue4) EndDefine
 
  #define _checkCondition_(checkValue) EndDefine

#endif

/*--------------------------------------------------------------------------*/
/* check initialised                                                        */
/*--------------------------------------------------------------------------*/
#ifdef WITH_CHECK_INITIALISED

  #define _checkInitialised_(eClass) \
    MemLib::vCheckInitialised(this,eClass); \
  EndDefine

  #define _checkInitialisedSize_(iSize) \
    MemLib::vCheckInitialised(this,iSize); \
  EndDefine

#else

  #define _checkInitialised_(eClass) EndDefine
  #define _checkInitialisedSize_(iSize) EndDefine

#endif

/*--------------------------------------------------------------------------*/
/* check the object has a magic marker as its first word                    */
/*--------------------------------------------------------------------------*/
#ifdef WITH_CORRECTNESS_ALGEBRA
#define _magic_() ASSERT(bValidMagic()); EndDefine
#else
#define _magic_() EndDefine
#endif

/*----------------------------------------------------------------------------*/
/* macro for standard bCorrectLayout() function                               */
/*----------------------------------------------------------------------------*/
#define __bCorrectLayout(CLASSNM,CN) \
bool CLASSNM::bCorrectLayout(void) \
{ \
  if (! bValidMagic()) \
    return false; \
  \
  if (! bIsClass(CN)) \
    return false; \
  \
  if (! aFooter()->bValidFooter()) \
    return false; \
  \
  return true; \
} \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard safe cast function                                      */
/*----------------------------------------------------------------------------*/
#define __pCastFunction(SPACENM,CLASSNM,CN) \
  CLASSNM* SPACENM::_p##CLASSNM(void* aAddr) \
  { \
    CLASSNM* pObject = reinterpret_cast<CLASSNM*>(aAddr); \
    RASSERT_TYPE(pObject,CN); \
    \
    return pObject; \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard operator new() function                                 */
/*----------------------------------------------------------------------------*/
#define __aNewFunction(CLASSNM,CN) \
  void* CLASSNM::operator new(size_t) \
  { \
    void* pObject = HeapMem::pAllocate(CN); \
  \
    return pObject; \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard operator delete() function                              */
/*----------------------------------------------------------------------------*/
#define __vDeleteFunction(CLASSNM,CN) \
  void CLASSNM::operator delete(void* aAddr) \
  { \
    CLASSNM* pObject = reinterpret_cast<CLASSNM*>(aAddr); \
   \
    HeapMem::vFree(pObject); \
  } \
EndDefine

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*--------------------------------------------------------------------------*/
/* check the object is of a known class                                     */
/*--------------------------------------------------------------------------*/
#ifdef WITH_CORRECTNESS_ALGEBRA
#define _class_(arg) ASSERT(eClass() == arg); EndDefine
#define _class2_(arg1,arg2) ASSERT(eClass() == arg1 || eClass() == arg2); EndDefine
#define _class3_(arg1,arg2,arg3) \
  ASSERT(eClass() == arg1 || eClass() == arg2 || eClass() == arg3); \
EndDefine
#else
#define _class_(arg)  EndDefine
#define _class2_(arg1,arg2)  EndDefine
#define _class3_(arg1,arg2,arg3)  EndDefine
#endif
