/*============================================================================*/
/* XF_Optimise_Promote_filter.hpp -- Individuate filter                            */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_Promote_filter : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    XF_Optimise_Promote_filter(CardIter* pCI);
   ~XF_Optimise_Promote_filter(void) noexcept(false);

    static void vXF_Individuate_filter(CardIter* pCI);

    void vXF_Individuate_filter(void);

  private:
    Footer* aFooter(void);

  };

  XF_Optimise_Promote_filter* _pXF_Optimise_Promote_filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
