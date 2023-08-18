/*============================================================================*/
/* XF_Cull_NonUsed_bound.hpp -- Cull non-used bound                           */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Cull_NonUsed_bound : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Cull_NonUsed_bound(CardIter* pCI);
   ~XF_Cull_NonUsed_bound(void) noexcept(false);

    static void vXF_Cull_NonUsed_bound(CardIter* pCI);

    void vXF_Cull_NonUsed_bound(void);

  private:
    Footer* aFooter(void);
  };

  XF_Cull_NonUsed_bound* _pXF_Cull_NonUsed_bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
