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
    
    void    operator =(Object* pObject);
  
    i8      nStackUsage(void);
    void    vToString(Stz* szDst);
  };
  
  PtrVal* _aPtrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
