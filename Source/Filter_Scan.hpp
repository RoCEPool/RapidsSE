/*============================================================================*/
/* Filter_Scan.hpp -- Scan Filter operand                                     */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Scan : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Scan(TokenVec* pTokenVec=nullptr);
   ~Filter_Scan(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Scan* _pFilter_Scan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
