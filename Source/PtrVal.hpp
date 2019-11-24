/*============================================================================*/
/* PtrVal.hpp -- Layout of a Pointer to a Proper Object when on Stack         */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class PtrVal : public StkVal
  {
  private:
    Object* m_pObj;

  public:
    void* operator new(size_t);
    
    PtrVal(void);
    
    void    operator =(Object* pObject);
    Object* pObj(void);

    i8      iAsInt(void);
    i8      nStackBytes(void);
    void    vToString(Stz* szDst);
  };
  
  PtrVal* _aPtrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
