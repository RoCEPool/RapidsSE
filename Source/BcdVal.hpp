/*============================================================================*/
/* BcdVal.hpp -- Bcd Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class BcdVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Bcd   m_wBcd;

  public:
    void* operator new(size_t);
    
  public:
    Bcd   wBcd(void);
    void  vBcd(Bcd wBcd);
  
    void vSetMax(void)                { m_wBcd.vSetMax(); }  // set max value
    void vSetMid(void)                { m_wBcd.vSetMid(); }  // set mid value
    void vSetMin(void)                { m_wBcd.vSetMin(); }  // set min value
    void vSetNull(void)              { m_wBcd.vSetNull(); }  // set to be null
    void vSetUsual(void)            { m_wBcd.vSetUsual(); }  // set usual value

    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline BcdVal* _aBcdVal(void* aAddr)
  {
    BcdVal* aBcdVal = reinterpret_cast<BcdVal*>(aAddr);
  
    RASSERT_TYPE(aBcdVal,CN_BCDVAL);
  
    return aBcdVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
