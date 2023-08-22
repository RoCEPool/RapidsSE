/*============================================================================*/
/* XF_Optimise_Late_projection.hpp -- Optimise max to top                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_Late_projection : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Late_projection(CardIter* pCI);
   ~XF_Optimise_Late_projection(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    void vOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Late_projection* _pXF_Optimise_Late_projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
