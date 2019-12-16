/*============================================================================*/
/* DteVal.hpp -- Dte Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class DteVal : public _Header
  {
  friend class Datums::_Datum_;
  
  private:
  union {
    Dte   m_wDte;
    i8    m_iDte;
  };

  public:
    void* operator new(size_t);
    
  public:
    Dte   wDte(void);
    void  vDte(Dte wDte);
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
