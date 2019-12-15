/*============================================================================*/
/* TmeVal.hpp -- Tme Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class TmeVal : public _Header
  {
  friend class Datums::Datum;
  
  private:
  union {
    Tme   m_wTme;
    i8    m_iInt;
  };

  public:
    void* operator new(size_t);
    
  public:
    Tme   wTme(void);
    void  vTme(Tme wTme);
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
