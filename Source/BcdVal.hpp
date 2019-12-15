/*============================================================================*/
/* BcdVal.hpp -- Bcd Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class BcdVal : public _Header
  {
  friend class Datums::Datum;
  
  private:
  union {
    Bcd   m_wBcd;
    i8    m_iBcd;
  };

  public:
    void* operator new(size_t);
    
  public:
    Bcd   wBcd(void);
    void  vBcd(Bcd wBcd);
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
