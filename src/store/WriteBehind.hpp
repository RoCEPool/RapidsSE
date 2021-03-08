/*============================================================================*/
/* WriteBehind.hpp -- Working WriteBehind Buffer                              */
/*============================================================================*/
#pragma once

namespace STORE
{
  class WriteBehind : public Object
  {
  private:
  
    RowPix* m_pRowPix;    // condensed stream def

    //HashSet    m_oHashSet; // using this as an interim step
    DataBuffer*  m_paBufferChain[C_MAX_PARTITIONS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    WriteBehind(void);
   ~WriteBehind(void) noexcept(false);
  
  public:
    
    DataBuffer* pBuffer(i8 i_Ptn);
    DataBuffer* pCompleted(i8 i_Ptn);
    DataBuffer* pDangling(i8 i_Ptn);
    Row* aNewDstRow(i8 i_Ptn, RowBuffer** ppRowBuffer, bool* pbNewBuffer);
  
    void vRowPix(RowPix* pRowPix);
  
  private:
    void vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer);

    Footer* aFooter(void);
  };

  WriteBehind* _pWriteBehind(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
