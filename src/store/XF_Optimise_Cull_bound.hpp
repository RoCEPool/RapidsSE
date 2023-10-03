/*============================================================================*/
/* XF_Optimise_Cull_bound.hpp -- Cull non-used bound                           */
/*============================================================================*/
#pragma once

#include "XF_OPTIMISE.hpp"

namespace STORE
{
  class XF_Optimise_Cull_bound : public XF_OPTIMISE
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Cull_bound(CardIter* pCI);
   ~XF_Optimise_Cull_bound(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bXF_Cull_NonUsed_bound(void);
    bool bXF_Cull_Used_bound(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Cull_bound* _pXF_Optimise_Cull_bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
