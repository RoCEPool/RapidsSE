/*============================================================================*/
/* IntVal.hpp -- Int Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class IntVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Int   m_wInt;

  public:
    void* operator new(size_t);
    
  public:
    i8    iInt(void);
    Int   wInt(void);
    void  vInt(i8 iInt);
    void  vInt(Int wInt);
  
    void vSetMax(void)                { m_wInt.vSetMax(); }  // set max value
    void vSetMid(void)                { m_wInt.vSetMid(); }  // set mid value
    void vSetMin(void)                { m_wInt.vSetMin(); }  // set min value
    void vSetNull(void)              { m_wInt.vSetNull(); }  // set to be null
    void vSetUsual(void)            { m_wInt.vSetUsual(); }  // set usual value

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
