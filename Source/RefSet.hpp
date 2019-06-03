/*============================================================================*/
/* RefSet.hpp -- Row-Ref Set                                                  */
/*============================================================================*/
#pragma once

namespace Mock
{
  class RefSet : public Object
  {
  public:
    i8    m_nRowRefs;
  
    i8    m_iaRowRef[1];
  
  public:
    void* operator new(size_t, void* aAddr);
    void  operator delete(void*);

    RefSet(i8 nRowRefs);
    ~RefSet(void) noexcept(false);
  
  };
  
  RefSet* _pRefSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
