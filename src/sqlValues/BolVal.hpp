/*============================================================================*/
/* BolVal.hpp -- Bol Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class BolVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Bol   m_wBol;

  public:
    void* operator new(size_t);
    
  public:
    Bol   wBol(void);
    void  vBol(Bol wBol);
  
    void vSetMax(void)                { m_wBol.vSetMax(); }  // set max value
    void vSetMid(void)                { m_wBol.vSetMid(); }  // set mid value
    void vSetMin(void)                { m_wBol.vSetMin(); }  // set min value
    void vSetNull(void)              { m_wBol.vSetNull(); }  // set to be null
    void vSetUsual(void)            { m_wBol.vSetUsual(); }  // set usual value

    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline BolVal* _aBolVal(void* aAddr)
  {
    BolVal* aBolVal = reinterpret_cast<BolVal*>(aAddr);
  
    RASSERT_TYPE(aBolVal,CN_BOLVAL);
  
    return aBolVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
