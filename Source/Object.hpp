/*============================================================================*/
/* Object.hpp -- Base Class for Objects in Buffer Pool                        */
/*============================================================================*/
#pragma once

namespace Pool
{
  class bn;
}

namespace CorrectnessAlgebra
{
  static const u8 C_MAGIC   = 0x4B696A4D; // MjiK
                              //K i j M
  
  class Object
  {
  protected:
    union {
      u4   m_uMagic;
      char m_sMagic[4];
    };
  private:
    eCN  m_eClass;
    bool m_bWordBody;
  
  public:
    Object(eCN eClass);
  
    i8  iBufferNo(void);
    Pool::bn nBufferNo(void);
  
    i8    iMagic(void) const;
    bool  bValidMagic(void);
  
    Stz* aClass(void);
  
    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* aFooter(void);
  
  /*--------------------------------------------------------------------------*/
  /* get class                                                                */
  /*--------------------------------------------------------------------------*/
  inline eCN eClass(void)
  {
    return m_eClass;
  }
  
  /*----------------------------------------------------------------------------*/
  /* set class                                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vSetClass(eCN eClass)
  {
    m_eClass = eClass;
  }
  
  /*----------------------------------------------------------------------------*/
  /* set magic                                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vSetMagic(void)
  {
    m_uMagic = C_MAGIC;
  }
  
  };
}

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

#ifdef __DEBUG_CHECKS

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
/* check the object has a magic marker as its first word                    */
/*--------------------------------------------------------------------------*/
#ifdef __MAGIC_CHECK
#define _magic_() ASSERT(iMagic() == C_MAGIC);
#else
#define _magic_()
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



/*--------------------------------------------------------------------------*/
/* check the object is of a known class                                     */
/*--------------------------------------------------------------------------*/
#ifdef __CLASS_CHECK
#define _class_(arg) ASSERT(eClass() == arg);
#define _class2_(arg1,arg2) ASSERT(eClass() == arg1 || eClass() == arg2);
#define _class3_(arg1,arg2,arg3) \
ASSERT(eClass() == arg1 || eClass() == arg2 || eClass() == arg3);
#else
#define _class_(arg)
#define _class2_(arg1,arg2)
#define _class3_(arg1,arg2,arg3)
#endif
