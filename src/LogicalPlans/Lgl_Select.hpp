/*============================================================================*/
/* Filter_Select.hpp -- Select LogicalOp operand                                 */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Select : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Select(TokenVec* pTokenVec=nullptr);
   ~Filter_Select(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Select* _pFilter_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
