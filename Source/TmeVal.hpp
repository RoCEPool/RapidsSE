/*============================================================================*/
/* IntVal.hpp -- Layout of an i8 Integer when on Stack                        */
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
    void  vTme(i8 iVal);
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
