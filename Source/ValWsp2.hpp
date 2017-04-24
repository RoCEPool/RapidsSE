/*============================================================================*/
/* ValWsp2.hpp - workspace for manipulating values (version 2)                */
/*============================================================================*/
#pragma once

namespace Values
{
  class ValWsp2 : public Value
  {
  private:
    WspFxd<C_64K> m_xWsp;
    Footer        m_xFooter;
  
  public:
    void* operator new(size_t);
    ValWsp2(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Str* aStr);
    
    i8    iAsInt(void);
    void vAssignBinary(Object* pObject);
  
    char* aWspArea(void);
  
    Footer* pFooter(void);
  };
  
  ValWsp2* _pValWsp2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
