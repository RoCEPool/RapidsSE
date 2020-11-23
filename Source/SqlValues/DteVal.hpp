/*============================================================================*/
/* DteVal.hpp -- Dte Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class DteVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Dte   m_wDte;

  public:
    void* operator new(size_t);
    
  public:
    Dte   wDte(void);
    void  vDte(Dte wDte);
  
    void vSetMax(void)                { m_wDte.vSetMax(); }  // set max value
    void vSetMid(void)                { m_wDte.vSetMid(); }  // set mid value
    void vSetMin(void)                { m_wDte.vSetMin(); }  // set min value
    void vSetNull(void)              { m_wDte.vSetNull(); }  // set to be null
    void vSetUsual(void)            { m_wDte.vSetUsual(); }  // set usual value

    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline DteVal* _aDteVal(void* aAddr)
  {
    DteVal* aDteVal = reinterpret_cast<DteVal*>(aAddr);
  
    RASSERT_TYPE(aDteVal,CN_DTEVAL);
  
    return aDteVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
