/*============================================================================*/
/* XF_Cull_Used_bound.hpp -- Cull used bound                                  */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Cull_Used_bound : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Cull_Used_bound(CardIter* pCI);
   ~XF_Cull_Used_bound(void) noexcept(false);

    static void vXF_Cull_Used_bound(CardIter* pCI);

    void vXF_Cull_Used_bound(void);

  private:
    Footer* aFooter(void);
  };

  XF_Cull_Used_bound* _pXF_Cull_Used_bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
