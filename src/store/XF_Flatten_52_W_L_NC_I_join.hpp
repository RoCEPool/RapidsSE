/*============================================================================*/
/* XF_Flatten_52_W_L_NC_I_join.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_52_W_L_NC_I_join : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_52_W_L_NC_I_join(CardIter* pCI);
   ~XF_Flatten_52_W_L_NC_I_join(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_52_W_L_NC_I_join* _pXF_Flatten_52_W_L_NC_I_join(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
