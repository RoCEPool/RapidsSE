/*============================================================================*/
/* XF_Flatten_Fold.hpp -- Flatten                                             */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"

namespace STORE
{
  class XF_Flatten_Fold : public XF_FLATTEN
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Fold(CardIter* pCI);
   ~XF_Flatten_Fold(void) noexcept(false);

    static void v10_F(CardIter* pCI);

    void v10_F(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Fold* _pXF_Flatten_Fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
