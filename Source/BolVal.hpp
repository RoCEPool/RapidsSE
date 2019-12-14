/*============================================================================*/
/* BolVal.hpp -- Bol Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class BolVal : public AnyVal
  {
  private:
  
  public:
    void* operator new(size_t);
    
  public:
    void  vBol(i8 iVal);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline BolVal* _aBolVal(void* aAddr)
  {
    BolVal* aBolVal = reinterpret_cast<BolVal*>(aAddr);
  
    RASSERT_TYPE(aBolVal,CN_INTVAL);
  
    return aBolVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
