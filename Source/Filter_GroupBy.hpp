/*============================================================================*/
/* Filter_GroupBy.hpp -- GroupBy LogicalOp operand                               */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_GroupBy : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_GroupBy(TokenVec* pTokenVec=nullptr);
   ~Filter_GroupBy(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_GroupBy* _pFilter_GroupBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
