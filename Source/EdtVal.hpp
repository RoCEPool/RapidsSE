/*============================================================================*/
/* IntVal.hpp -- Layout of an i8 Integer when on Stack                        */
/*============================================================================*/
#pragma once

namespace SqlValues
{
  class EdtVal : public AnyVal
  {
  private:
  
  public:
    void* operator new(size_t);
    
  public:
    void  vEdt(i8 iVal);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline EdtVal* _aEdtVal(void* aAddr)
  {
    EdtVal* aEdtVal = reinterpret_cast<EdtVal*>(aAddr);
  
    RASSERT_TYPE(aEdtVal,CN_INTVAL);
  
    return aEdtVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
