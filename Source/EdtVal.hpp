/*============================================================================*/
/* EdtVal.hpp -- Edt Value                                                    */
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
    Edt   wEdt(void);
    void  vEdt(Edt wEdt);
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
