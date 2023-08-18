/*============================================================================*/
/* XF_Flatten_Delete_flat.hpp -- Delete flat                                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Delete_flat : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Delete_flat(CardIter* pCI);
   ~XF_Flatten_Delete_flat(void) noexcept(false);

    static void vXF_Delete_flat(CardIter* pCI);

    void vXF_Delete_flat(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Delete_flat* _pXF_Flatten_Delete_flat(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
