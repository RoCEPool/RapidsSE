/*============================================================================*/
/* XF_Flatten_40_W_L_C_E.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"

namespace STORE
{
  class XF_Flatten_40_W_L_C_E : public XF_FLATTEN
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_40_W_L_C_E(CardIter* pCI);
   ~XF_Flatten_40_W_L_C_E(void) noexcept(false);

    static void v40_W_L_C_E(CardIter* pCI);

    void v40_W_L_C_E(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_40_W_L_C_E* _pXF_Flatten_40_W_L_C_E(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
