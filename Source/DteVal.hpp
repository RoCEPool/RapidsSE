/*============================================================================*/
/* DteVal.hpp -- Dte Value                                                    */
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
  
    RASSERT_TYPE(aDteVal,CN_INTVAL);
  
    return aDteVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
