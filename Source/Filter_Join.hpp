/*============================================================================*/
/* Filter_Join.hpp -- Join Filter operand                                     */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Join : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Join(TokenVec* pTokenVec=nullptr);
   ~Filter_Join(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Join* _pFilter_Join(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
