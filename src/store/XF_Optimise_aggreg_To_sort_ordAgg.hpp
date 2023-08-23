/*============================================================================*/
/* XF_Optimise_aggreg_To_sort_ordAgg.hpp -- Optimise aggreg to sort + ordagg  */
/*============================================================================*/
#pragma once

#include "XF_Optimise.hpp"

namespace STORE
{
  class XF_Optimise_aggreg_To_sort_ordAgg : public XF_Optimise
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_aggreg_To_sort_ordAgg(CardIter* pCI);
   ~XF_Optimise_aggreg_To_sort_ordAgg(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_aggreg_To_sort_ordAgg* _pXF_Optimise_aggreg_To_sort_ordAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
