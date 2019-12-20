/*============================================================================*/
/* TmeVal.hpp -- Tme Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class TmeVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Tme   m_wTme;

  public:
    void* operator new(size_t);
    
  public:
    Tme   wTme(void);
    void  vTme(Tme wTme);
  
    void vSetMax(void)                { m_wTme.vSetMax(); }  // set max value
    void vSetMid(void)                { m_wTme.vSetMid(); }  // set mid value
    void vSetMin(void)                { m_wTme.vSetMin(); }  // set min value
    void vSetNull(void)              { m_wTme.vSetNull(); }  // set to be null
    void vSetUsual(void)            { m_wTme.vSetUsual(); }  // set usual value

    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline TmeVal* _aTmeVal(void* aAddr)
  {
    TmeVal* aTmeVal = reinterpret_cast<TmeVal*>(aAddr);
  
    RASSERT_TYPE(aTmeVal,CN_TMEVAL);
  
    return aTmeVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
