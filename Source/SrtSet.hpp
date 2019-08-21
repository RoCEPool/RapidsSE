/*============================================================================*/
/* SrtSet.hpp -- Sort Set                                                     */
/*============================================================================*/
#pragma once

namespace Rows
{
  class SrtSet : public Header
  {
  public:
    i8 m_xKey;        // 1 word key
    i8 m_nRowRefs;    // the number of row refs in this ref set
    i8 m_iaRowRef[1]; // array of row refs
  
  public:
    void* operator new(size_t, void* aAddr);
    void  operator delete(void*);
  
    SrtSet(i8 nRowRefs);
    ~SrtSet(void) noexcept(false);
  
    static i8 nBytesReq(i8 nRowRefs);
    
    void vCopy(SrtSet* aSrtSet);
    i8   iKey(void);
    void vKey(i8 xKey);
    i8   nRowRefs(void);
  
    i8   iRowRef(i8 i_RowRef);
    void vRowRef(i8 i_RowRef, i8 iRowRef);
  
    i8   nSizeBytes(void);
  };
  
  SrtSet* _pSrtSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
