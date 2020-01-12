/*============================================================================*/
/* SrtSet.hpp -- Sort Set                                                     */
/*============================================================================*/
#pragma once

namespace SortBuffers
{
  class SrtSet : public Header
  {
  public:
    i8 m_xKey;        // 1 word key
    i8 m_iaRowRef[1]; // array of row refs
  
  public:
    void* operator new(size_t, void* aAddr);
    void  operator delete(void*);
  
    SrtSet(i8 nRowRefs);
    ~SrtSet(void) noexcept(false);
    
    /*----------------------------------------------------------------------------*/
    /* copy from srtSet to this srtSet                                            */
    /*----------------------------------------------------------------------------*/
    inline void vCopy(SrtSet* pSrtSet, i8 nRowRefs)
    {
      m_xKey = pSrtSet->iKey();
      
      for (i8 i_RowRef = 0; i_RowRef < nRowRefs; i_RowRef++)
        m_iaRowRef[i_RowRef] = pSrtSet->m_iaRowRef[i_RowRef];
    }
    
    /*----------------------------------------------------------------------------*/
    /* get key from this srt set                                                  */
    /*----------------------------------------------------------------------------*/
    inline i8 iKey(void)
    {
      return m_xKey;
    }
    
    /*----------------------------------------------------------------------------*/
    /* set key in this srt set                                                    */
    /*----------------------------------------------------------------------------*/
    inline void vKey(i8 xKey)
    {
      m_xKey = xKey;
    }
    
    /*----------------------------------------------------------------------------*/
    /* get a row ref from this srt set                                            */
    /*----------------------------------------------------------------------------*/
    inline i8 iRowRef(i8 i_RowRef)
    {
      return m_iaRowRef[i_RowRef];
    }
    
    /*----------------------------------------------------------------------------*/
    /* set a row ref in this srt set                                              */
    /*----------------------------------------------------------------------------*/
    inline void vRowRef(i8 i_RowRef, i8 iRowRef)
    {
      m_iaRowRef[i_RowRef] = iRowRef;
    }

  };
  
  SrtSet* _pSrtSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
