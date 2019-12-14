/*============================================================================*/
/* IntVal.hpp -- Ivl Value                                                    */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class IvlVal : public AnyVal
  {
  private:
  
  public:
    void* operator new(size_t);
    
  public:
    void  vIvl(i8 iVal);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline IvlVal* _aIvlVal(void* aAddr)
  {
    IvlVal* aIvlVal = reinterpret_cast<IvlVal*>(aAddr);
  
    RASSERT_TYPE(aIvlVal,CN_INTVAL);
  
    return aIvlVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
