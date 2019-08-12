/*============================================================================*/
/* Filter_OrderBy.hpp -- Where Filter operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Filter_OrderBy : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_OrderBy(void);
   ~Filter_OrderBy(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_OrderBy* _pFilter_OrderBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
