/*============================================================================*/
/* Filter.hpp -- Filter RsbBuffers into new RsbBuffers                        */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class FilterRsb : public Object
  {
  private:
      
    Pot*       m_pPot;  

    RsbBuffer* m_pFromRsbBuffer; // the
  
    RsbBuffer* m_pIntoRsbBuffer; // the
    RowSlab*   m_pIntoRowSlab;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    FilterRsb(void);
   ~FilterRsb(void) noexcept(false);
  
    void vFilter(RsbBuffer* pRsbBuffer, Expr* pExpr, Pot* pPot);
    
  private:
    void vCopyRsb(Rsb* aRsbSrc);
    void vNewDstRsb(void);

    Footer* aFooter(void);
  };

  FilterRsb* _pFilterRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
