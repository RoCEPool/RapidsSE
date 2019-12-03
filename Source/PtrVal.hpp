/*============================================================================*/
/* PtrVal.hpp -- Layout of a Pointer to a Proper Object when on Stack         */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class PtrVal : public AnyVal
  {
  private:
 
  public:
    void* operator new(size_t);
    
  public:
    void  vObj(Object* pObject);
    i8    nStackUsage(void);
    void  vToStz(Stz* szDst);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline PtrVal* _aPtrVal(void* aAddr)
  {
    PtrVal* aPtrVal = reinterpret_cast<PtrVal*>(aAddr);
  
    RASSERT_TYPE(aPtrVal,CN_PTRVAL);
  
    return aPtrVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
