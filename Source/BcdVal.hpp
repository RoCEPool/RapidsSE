/*============================================================================*/
/* BcdVal.hpp -- Bcd Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class BcdVal : public AnyVal
  {
  private:
  
  public:
    void* operator new(size_t);
    
  public:
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
  
    RASSERT_TYPE(aBcdVal,CN_INTVAL);
  
    return aBcdVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
