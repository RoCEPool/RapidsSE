/*============================================================================*/
/* Filter_Select.hpp -- Where Filter operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Filter_Select : public Filter
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filter_Select(void);
   ~Filter_Select(void) noexcept(false);

    Footer* aFooter(void);
  };

  Filter_Select* _pFilter_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
