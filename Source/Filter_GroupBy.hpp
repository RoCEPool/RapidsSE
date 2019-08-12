/*============================================================================*/
/* Filter_GroupBy.hpp -- Where Filter operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Filter_GroupBy : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_GroupBy(void);
   ~Filter_GroupBy(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_GroupBy* _pFilter_GroupBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
