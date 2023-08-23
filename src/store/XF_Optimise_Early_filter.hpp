/*============================================================================*/
/* XF_Optimise_Early_filter.hpp -- Optimise early filter                      */
/*============================================================================*/
#pragma once

#include "XF_OPTIMISE.hpp"

namespace STORE
{
  class XF_Optimise_Early_filter : public XF_OPTIMISE
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Early_filter(CardIter* pCI);
   ~XF_Optimise_Early_filter(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Early_filter* _pXF_Optimise_Early_filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
