/*============================================================================*/
/* Filter_Limit.hpp -- Limit LogicalOp operand                                   */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Limit : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Limit(TokenVec* pTokenVec=nullptr);
   ~Filter_Limit(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Limit* _pFilter_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
