/*============================================================================*/
/* XF_Flatten_Derive_bounds.hpp -- Derive Bounds (from filter)                        */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Derive_bounds : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Derive_bounds(CardIter* pCI);
   ~XF_Flatten_Derive_bounds(void) noexcept(false);

    static void vXF_Derive_bounds(CardIter* pCI);

    void vXF_Derive_bounds(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Derive_bounds* _pXF_Flatten_Derive_bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
