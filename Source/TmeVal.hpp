/*============================================================================*/
/* TmeVal.hpp -- Tme Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class TmeVal : public AnyVal
  {
  private:
  
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
  
    RASSERT_TYPE(aTmeVal,CN_INTVAL);
  
    return aTmeVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
