/*============================================================================*/
/* TspVal.hpp -- Tsp Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class TspVal : public _Header
  {
  friend class Datums::Datum_h;
  template <i8 SPACE> friend class Datums::Datum_t;

  private:
    Tsp   m_wTsp;

  public:
    void* operator new(size_t);
    
  public:
    Tsp   wTsp(void);
    void  vTsp(Tsp wTsp);
  
    void vSetMax(void)                { m_wTsp.vSetMax(); }  // set max value
    void vSetMid(void)                { m_wTsp.vSetMid(); }  // set mid value
    void vSetMin(void)                { m_wTsp.vSetMin(); }  // set min value
    void vSetNull(void)              { m_wTsp.vSetNull(); }  // set to be null
    void vSetUsual(void)            { m_wTsp.vSetUsual(); }  // set usual value

    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline TspVal* _aTspVal(void* aAddr)
  {
    TspVal* aTspVal = reinterpret_cast<TspVal*>(aAddr);
  
    RASSERT_TYPE(aTspVal,CN_TSPVAL);
  
    return aTspVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
