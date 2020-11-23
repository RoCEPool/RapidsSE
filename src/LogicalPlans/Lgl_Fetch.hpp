/*============================================================================*/
/* Filter_Fetch.hpp -- Fetch LogicalOp operand                                   */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Fetch : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Fetch(TokenVec* pTokenVec=nullptr);
   ~Filter_Fetch(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Fetch* _pFilter_Fetch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
