/*============================================================================*/
/* AggregateRsb.hpp -- Aggregate RsbBuffers into new RsbBuffers               */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class AggregateRsb : public Object
  {
  private:
    Pot*       m_pPot;          // working pot of destination buffers
    RowSlab*   m_pDstRowSlab;   // current destination slab (in pot)
    RsbBuffer* m_pSrcRsbBuffer; // source rsb buffer
    StreamDef* m_pStreamDefOut; // output stream def
  
    bool       m_bPixInited;
    StreamPix  m_oStreamPix;    // condensed stream def
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    AggregateRsb(Pot* pPot, StreamDef* pStreamDefOut);
   ~AggregateRsb(void) noexcept(false);
  
    void vAggregateCpy(Pot* pPot, IndexTree* pIndexTree);
    void vAggregateGby(Pot* pPot, IndexTree* pIndexTree, RsbBuffer* pSrcRsbBuffer);
    void vNewDstRsb(void);
    
  private:
    void vCopyIntoDst(Rsb* aSrcRsbc);
    
    Footer* aFooter(void);
  };

  AggregateRsb* _pAggregateRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
