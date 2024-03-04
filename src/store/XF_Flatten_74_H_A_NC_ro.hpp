/*============================================================================*/
/* XF_Flatten_74_H_A_NC_ro.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_74_H_A_NC_ro : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_74_H_A_NC_ro(CardIter* pCI);
   ~XF_Flatten_74_H_A_NC_ro(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vXF_Flatten_74_H_A_NC_ro(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_74_H_A_NC_ro* _pXF_Flatten_74_H_A_NC_ro(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
