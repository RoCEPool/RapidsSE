/*============================================================================*/
/* Filter_Output.hpp -- Output Filter operand                                 */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class Filter_Output : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Output(TokenVec* pTokenVec=nullptr);
   ~Filter_Output(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Output* _pFilter_Output(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
