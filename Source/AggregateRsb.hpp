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
    
    AggregateRsb(void);
   ~AggregateRsb(void) noexcept(false);
  
    void vAggregate(Pot* pPot, RsbBuffer* pSrcRsbBuffer,
                  IndexTree* pIndexTree, Expr* pExpr, StreamDef* pStreamDefOut);
  private:
    void vCopyIntoDst(Rsb* aSrcRsbc);
    void vNewDstRsb(void);

    Footer* aFooter(void);
  };

  AggregateRsb* _pAggregateRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
