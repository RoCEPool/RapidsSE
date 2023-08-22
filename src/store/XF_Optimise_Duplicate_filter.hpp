/*============================================================================*/
/* XF_Optimise_Duplicate_filter.hpp -- Optimise max to top                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_Duplicate_filter : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Duplicate_filter(CardIter* pCI);
   ~XF_Optimise_Duplicate_filter(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    void vOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Duplicate_filter* _pXF_Optimise_Duplicate_filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
