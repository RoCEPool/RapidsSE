/*============================================================================*/
/* ToPtnRsb.hpp -- Convert rsb buffers to partitioned row slab buffers        */
/*============================================================================*/
#pragma once

namespace Load
{
  class ToPtnRsb : public Object
  {
  private:
  
    HashSet  m_oHashSet;
  
    RowBuffer* m_pFromRsbBuffer; // the

    RowBuffer*   m_pIntoRowBuffer;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToPtnRsb(void);
   ~ToPtnRsb(void) noexcept(false);
  
    void vConvert(RowBuffer* pRsbBuffer);
  
    RowBuffer* pCompleted(i8 i_Ptn);
    RowBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vCopyRow(i8 i_Ptn, Row* aRowSrc);
    void vNewDstRow(i8 i_Ptn);

    Footer* aFooter(void);
  };

  ToPtnRsb* _pToPtnRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
