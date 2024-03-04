/*============================================================================*/
/* XF_Flatten_52_W_L_NC_I_exists.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN_ABS.hpp"

namespace STORE
{
  class XF_Flatten_52_W_L_NC_I_exists : public XF_FLATTEN_ABS
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_52_W_L_NC_I_exists(CardIter* pCI);
   ~XF_Flatten_52_W_L_NC_I_exists(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_52_W_L_NC_I_exists* _pXF_Flatten_52_W_L_NC_I_exists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
