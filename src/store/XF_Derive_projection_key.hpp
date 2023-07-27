/*============================================================================*/
/* XF_Derive_projection_key.hpp -- Derive projection key                      */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Derive_projection_key : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Derive_projection_key(CardIter* pCI);
   ~XF_Derive_projection_key(void) noexcept(false);

    static void vXF_Derive_projection_key(CardIter* pCI);

    void vXF_Derive_projection_key(void);

  private:
    Footer* aFooter(void);
  };

  XF_Derive_projection_key* _pXF_Derive_projection_key(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
