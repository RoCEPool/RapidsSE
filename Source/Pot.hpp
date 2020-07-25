/*============================================================================*/
/* Pot.hpp -- Working Pot of Buffers                                          */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class Pot : public Object
  {
  private:
  
    StreamPix* m_pStreamPix;    // condensed stream def

    HashSet    m_oHashSet; // using this as an interim step
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pot(void);
   ~Pot(void) noexcept(false);
  
  public:
    DataBuffer* pBuffer(void);
    DataBuffer* pCompleted(void);
    DataBuffer* pDangling(void);
    Rsb* aNewDstRsb(RowSlab** ppRowSlab);
    void vStreamPix(StreamPix* pStreamPix);
  
  private:
    void vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer);

    Footer* aFooter(void);
  };

  Pot* _pPot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
