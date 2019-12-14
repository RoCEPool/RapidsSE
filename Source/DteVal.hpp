/*============================================================================*/
/* IntVal.hpp -- Layout of an i8 Integer when on Stack                        */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class DteVal : public AnyVal
  {
  private:
  
  public:
    void* operator new(size_t);
    
  public:
    void  vDte(i8 iVal);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline DteVal* _aDteVal(void* aAddr)
  {
    DteVal* aDteVal = reinterpret_cast<DteVal*>(aAddr);
  
    RASSERT_TYPE(aDteVal,CN_INTVAL);
  
    return aDteVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
