/*============================================================================*/
/* Filter_Where.hpp -- Where LogicalOp operand                                   */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Where : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Where(TokenVec* pTokenVec=nullptr);
   ~Filter_Where(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Where* _pFilter_Where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
