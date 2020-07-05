/*============================================================================*/
/* Filter.hpp -- Filter RsbBuffers into new RsbBuffers                        */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class FilterRsb : public Object
  {
  private:
    Pot*       m_pPot;          // working pot of destination buffers
    RsbBuffer* m_pSrcRsbBuffer; // source rsb buffer
    RowSlab*   m_pDstRowSlab;   // current destination slab (in pot)
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    FilterRsb(void);
   ~FilterRsb(void) noexcept(false);
  
    void vFilter(RsbBuffer* pSrcRsbBuffer, Expr* pExpr, Pot* pPot);
    
  private:
    void vCopyIntoDst(Rsb* aSrcRsbc);
    void vNewDstRsb(void);

    Footer* aFooter(void);
  };

  FilterRsb* _pFilterRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
