/*============================================================================*/
/* XF_as_Review.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_as_Review : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_as_Review(CardIter* pCI);
   ~XF_as_Review(void) noexcept(false);

    static void vXF_as_Review(CardIter* pCI);

    void vXF_as_Review(void);

  private:
    Footer* aFooter(void);
  };

  XF_as_Review* _pXF_as_Review(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
