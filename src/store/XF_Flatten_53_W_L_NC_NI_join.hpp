/*============================================================================*/
/* XF_Flatten_53_W_L_NC_NI_join.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_53_W_L_NC_NI_join : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_53_W_L_NC_NI_join(CardIter* pCI);
   ~XF_Flatten_53_W_L_NC_NI_join(void) noexcept(false);

    static void vXF_Flatten_53_W_L_NC_NI_join(CardIter* pCI);

    void vXF_Flatten_53_W_L_NC_NI_join(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_53_W_L_NC_NI_join* _pXF_Flatten_53_W_L_NC_NI_join(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
