/*============================================================================*/
/* FltVal.hpp -- Flt Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class FltVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Flt   m_wFlt;
 
  public:
    void* operator new(size_t);
    
  public:
    Flt   wFlt(void);
    void  vFlt(Flt wFlt);
  
    void vSetMax(void)                { m_wFlt.vSetMax(); }  // set max value
    void vSetMid(void)                { m_wFlt.vSetMid(); }  // set mid value
    void vSetMin(void)                { m_wFlt.vSetMin(); }  // set min value
    void vSetNull(void)              { m_wFlt.vSetNull(); }  // set to be null
    void vSetUsual(void)            { m_wFlt.vSetUsual(); }  // set usual value

    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline FltVal* _aFltVal(void* aAddr)
  {
    FltVal* aFltVal = reinterpret_cast<FltVal*>(aAddr);
  
    RASSERT_TYPE(aFltVal,CN_FLTVAL);
  
    return aFltVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
