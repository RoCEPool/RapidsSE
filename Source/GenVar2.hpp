/*============================================================================*/
/* GenVar2.hpp - Generic Variable                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  class GenVar2 : public Object
  {
  private:

    Wsp_t<C_WSP_GENVAR> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);

    GenVar2(void);
   ~GenVar2(void) noexcept(false);

    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Stz* szStr);

    i8      iInt(void);
    IntVal* aNestedIntVal(void);
    PtrVal* aNestedPtrVal(void);
    StrVal* aStrVal(void);
    Value*  aNestedValue(void);

    Footer* aFooter(void);
  };

  GenVar2* _pGenVar2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
