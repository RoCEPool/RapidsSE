/*============================================================================*/
/* Filter_SubQry.hpp -- SubQry Filter operand                                 */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_SubQry : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_SubQry(TokenVec* pTokenVec=nullptr);
   ~Filter_SubQry(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_SubQry* _pFilter_SubQry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
