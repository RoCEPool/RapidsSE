/*============================================================================*/
/* XF_Flatten_24_W_A_C_ro.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"

namespace STORE
{
  class XF_Flatten_24_W_A_C_ro : public XF_FLATTEN
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_24_W_A_C_ro(CardIter* pCI);
   ~XF_Flatten_24_W_A_C_ro(void) noexcept(false);

    static void v24_W_A_C_ro(CardIter* pCI);

    void v24_W_A_C_ro(void);

  private:

    Footer* aFooter(void);
  };

  XF_Flatten_24_W_A_C_ro* _pXF_Flatten_24_W_A_C_ro(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
