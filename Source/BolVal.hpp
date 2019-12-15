/*============================================================================*/
/* BolVal.hpp -- Bol Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class BolVal : public _Header
  {
  friend class Datums::Datum;
  
  private:
  union {
    Bol   m_wBol;
    i8    m_iBol;
  };

  public:
    void* operator new(size_t);
    
  public:
    Bol   wBol(void);
    void  vBol(Bol wBol);
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
