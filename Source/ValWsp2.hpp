/*============================================================================*/
/* ValWsp2.hpp - workspace for manipulating values (version 2)                */
/*============================================================================*/
#pragma once

namespace Values
{
  // the value workspace
  class ValWsp2 : public Object
  {
  private:
    union {
      WspFxd<C_64K> m_xWsp;         // workspace
      ValSee        m_xNestedValue; // inspector
    };
    Footer          m_xFooter;
  
  public:
    void* operator new(size_t);
    ValWsp2(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Str* aStr);
  
    char*  aWspArea(void);
    Value* pNestedValue(void);
  
    i8      iNestedAsInt(void);
    IntVal* pNestedIntVal(void);
    PtrVal* pNestedPtrVal(void);
    StrVal* pNestedStrVal(void);
  
    Footer* pFooter(void);
  };
  
  ValWsp2* _pValWsp2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
