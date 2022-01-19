/*============================================================================*/
/* Object.hpp -- Proper Object in system                                      */
/*============================================================================*/
#pragma once

#include "eRF.h"

namespace BASE
{
  class Object : public Header
  {
  private:
  
  public:
    Object(eCN eClass);
  
    static bool bInspect(Object* pObject);
    static void vRepair(Object* pObject);

    Footer* aCalcFooterAddress(void);
    Stz* szClassNm(void);
    bool bInspectObject(void);
    Object* pMemoryCopy(void);
    void vMemoryDump(void);
    bool bMemoryLog(void);
    void vRunPostRecv(void);
    i8   nSizeOfObject(void);
    eEC  eValidObject(void);
    i8   iWhenAllocated(void);
  
    Footer* aFooter(void);
  };


  //
  // declare a function pointer to any member function of Object
  //   n.b. Object is non-virtual,
  //        so fpMEMBERFN can be cast to a sub-class of Object
  //
  typedef void (Object::*fpMEMBERFN)(void);

  Object* _pObject(void* aAddr);

} // namespace BASE

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


#ifdef WITH_CORRECTNESS_ALGEBRA

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

  #define _checkInitialised_(eClass)    (void)eClass EndDefine
  #define _checkInitialisedSize_(iSize) (void)iSize  EndDefine

#endif

/*--------------------------------------------------------------------------*/
/* check the object has a magic marker as its first word                    */
/*--------------------------------------------------------------------------*/
#ifdef WITH_CORRECTNESS_ALGEBRA
#define _magic_() ASSERT(bIsMagicValid()); EndDefine
#else
#define _magic_() EndDefine
#endif

/*----------------------------------------------------------------------------*/
/* macro for standard safe assert function                                    */
/*----------------------------------------------------------------------------*/
#define __AssertFunction(SPACENM,CLASSNM,CN) \
  void SPACENM::_##CLASSNM(void* aAddr) \
  { \
    CLASSNM* pObject = reinterpret_cast<CLASSNM*>(aAddr); \
    RASSERT_TYPE(pObject,CN); \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard bCorrectLayout() function                               */
/*----------------------------------------------------------------------------*/
#define __bCorrectLayout(CLASSNM,CN) \
bool CLASSNM::bCorrectLayout(void) \
{ \
  if (! bIsMagicValid()) \
    return false; \
  \
  if (! bIsClassNo(CN)) \
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
/* macro for test is class                                                    */
/*----------------------------------------------------------------------------*/
#define __bIsClass(SPACENM,CLASSNM,CN) \
bool SPACENM::_bIs##CLASSNM(void* aAddr) \
{ \
  Object* pObject = reinterpret_cast<Object*>(aAddr); \
  \
  if (CN == pObject->eGetClassNo()) \
    return true; \
  \
  return false; \
} \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard operator new() function                                 */
/*----------------------------------------------------------------------------*/
#define __aNewFunction(CLASSNM,CN) \
  void* CLASSNM::operator new(size_t) \
  { \
    void* pObject = Heaps::pAllocate(CN); \
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
    Heaps::vFree(pObject); \
  } \
EndDefine

/*--------------------------------------------------------------------------*/
/* check the object is of a known class                                     */
/*--------------------------------------------------------------------------*/
#ifdef WITH_CORRECTNESS_ALGEBRA

#define _class_(arg) \
  RASSERT(eGetClassNo() == arg); \
EndDefine

#define _class2_(arg1,arg2) \
  RASSERT(eGetClassNo() == arg1 || eGetClassNo() == arg2); \
EndDefine

#define _class3_(arg1,arg2,arg3) \
  RASSERT(eGetClassNo() == arg1 || \
          eGetClassNo() == arg2 || \
          eGetClassNo() == arg3); \
EndDefine

#else

#define _class_(arg)  EndDefine
#define _class2_(arg1,arg2)  EndDefine
#define _class3_(arg1,arg2,arg3)  EndDefine

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
