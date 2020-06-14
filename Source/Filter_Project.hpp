/*============================================================================*/
/* Filter_Project.hpp -- Project Filter operand                               */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Project : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Project(TokenVec* pTokenVec=nullptr);
   ~Filter_Project(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Project* _pFilter_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
