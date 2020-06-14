/*============================================================================*/
/* Filter_Zed.hpp -- Zed LogicalOp operand                                       */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Zed : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Zed(TokenVec* pTokenVec=nullptr);
   ~Filter_Zed(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Zed* _pFilter_Zed(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
