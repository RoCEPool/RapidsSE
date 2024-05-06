/*============================================================================*/
/* XF_Flatten_Delete_fold.hpp -- Delete fold                                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Delete_fold : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Delete_fold(CardIter* pCI);
   ~XF_Flatten_Delete_fold(void) noexcept(false);

    static void vXF_Delete_fold(CardIter* pCI);

    void vXF_Delete_fold(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Delete_fold* _pXF_Flatten_Delete_fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
