/*============================================================================*/
/* XF_Optimise_sort_limit_All_Levels.hpp -- Optimise max to top                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_sort_limit_All_Levels : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_sort_limit_All_Levels(CardIter* pCI);
   ~XF_Optimise_sort_limit_All_Levels(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    void vOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_sort_limit_All_Levels* _pXF_Optimise_sort_limit_All_Levels(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
