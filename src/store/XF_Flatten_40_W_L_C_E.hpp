/*============================================================================*/
/* XF_Flatten_40_W_L_C_E.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_40_W_L_C_E : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_40_W_L_C_E(CardIter* pCI);
   ~XF_Flatten_40_W_L_C_E(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vFlatten(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_40_W_L_C_E* _pXF_Flatten_40_W_L_C_E(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
