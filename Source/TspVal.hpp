/*============================================================================*/
/* IntVal.hpp -- Layout of an i8 Integer when on Stack                        */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class TspVal : public AnyVal
  {
  private:
  
  public:
    void* operator new(size_t);
    
  public:
    void  vTsp(i8 iVal);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline TspVal* _aTspVal(void* aAddr)
  {
    TspVal* aTspVal = reinterpret_cast<TspVal*>(aAddr);
  
    RASSERT_TYPE(aTspVal,CN_INTVAL);
  
    return aTspVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
