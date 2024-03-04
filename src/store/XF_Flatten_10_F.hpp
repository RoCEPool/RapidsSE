/*============================================================================*/
/* XF_Flatten_10_F.hpp -- Flatten                                             */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_10_F : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_10_F(CardIter* pCI);
   ~XF_Flatten_10_F(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vFlatten(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_10_F* _pXF_Flatten_10_F(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
