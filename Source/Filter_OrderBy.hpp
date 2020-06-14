/*============================================================================*/
/* Filter_OrderBy.hpp -- OrderBy Filter operand                               */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_OrderBy : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_OrderBy(TokenVec* pTokenVec=nullptr);
   ~Filter_OrderBy(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_OrderBy* _pFilter_OrderBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
