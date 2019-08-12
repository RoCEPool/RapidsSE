/*============================================================================*/
/* Filter_From.hpp -- Where Filter operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Filter_From : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_From(void);
   ~Filter_From(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_From* _pFilter_From(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
