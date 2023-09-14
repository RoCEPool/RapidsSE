/*============================================================================*/
/* XF_Optimise_max_To_top.hpp -- Optimise max to top                          */
/*============================================================================*/
#pragma once

#include "XF_OPTIMISE.hpp"

namespace STORE
{
  class XF_Optimise_max_To_top : public XF_OPTIMISE
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_max_To_top(CardIter* pCI);
   ~XF_Optimise_max_To_top(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_max_To_top* _pXF_Optimise_max_To_top(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
