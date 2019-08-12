/*============================================================================*/
/* Filter_Limit.hpp -- Where Filter operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Filter_Limit : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Limit(void);
   ~Filter_Limit(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Limit* _pFilter_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
