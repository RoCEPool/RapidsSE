/*============================================================================*/
/* Filter_From.hpp -- From LogicalOp operand                                     */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_From : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_From(TokenVec* pTokenVec=nullptr);
   ~Filter_From(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_From* _pFilter_From(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
