/*============================================================================*/
/* XF_Optimise_sortDesc_limit_to_top.hpp -- Optimise max to top                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_sortDesc_limit_to_top : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_sortDesc_limit_to_top(CardIter* pCI);
   ~XF_Optimise_sortDesc_limit_to_top(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    void vOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_sortDesc_limit_to_top* _pXF_Optimise_sortDesc_limit_to_top(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
