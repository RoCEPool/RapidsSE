/*============================================================================*/
/* IntVal.hpp -- Int Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class IntVal : public _Header
  {
  friend class Datums::Datum_abstract;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
  union {
    Int   m_wInt;
    i8    m_iInt;
  };

  public:
    void* operator new(size_t);
    
  public:
    i8    iInt(void);
    Int   wInt(void);
    void  vInt(i8 iInt);
    void  vInt(Int wInt);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline IntVal* _aIntVal(void* aAddr)
  {
    IntVal* aIntVal = reinterpret_cast<IntVal*>(aAddr);
  
    RASSERT_TYPE(aIntVal,CN_INTVAL);
  
    return aIntVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
