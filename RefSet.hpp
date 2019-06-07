/*============================================================================*/
/* RefSet.hpp -- Row-Ref Set                                                  */
/*============================================================================*/
#pragma once

namespace Data
{
  class RefSet : public Header
  {
  public:
    i8 m_nRowRefs;    // the number of row refs in this ref set
    i8 m_iaRowRef[1]; // array of row refs
  
  public:
    void* operator new(size_t, void* aAddr);
    void  operator delete(void*);
  
    RefSet(i8 nRowRefs);
    ~RefSet(void) noexcept(false);
  
    static i8 nBytesReq(i8 nRowRefs);
  
    i8   nRowRefs(void);
  
    i8   iRowRef(i8 i_RowRef);
    void vRowRef(i8 i_RowRef, i8 iRowRef);
  };
  
  RefSet* _pRefSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
