/*============================================================================*/
/* Filter_Inflate.hpp -- Inflate LogicalOp operand                               */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Inflate : public LogicalOp
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Inflate(TokenVec* pTokenVec=nullptr);
   ~Filter_Inflate(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Inflate* _pFilter_Inflate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
