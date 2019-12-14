/*============================================================================*/
/* IntVal.hpp -- Layout of an i8 Integer when on Stack                        */
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
    void  vBcd(i8 iVal);
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
