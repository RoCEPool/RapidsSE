/*============================================================================*/
/* XF_Flatten_32_W_A_NC_I.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_32_W_A_NC_I : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_32_W_A_NC_I(CardIter* pCI);
   ~XF_Flatten_32_W_A_NC_I(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vXF_Flatten_32_W_A_NC_I(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_32_W_A_NC_I* _pXF_Flatten_32_W_A_NC_I(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
