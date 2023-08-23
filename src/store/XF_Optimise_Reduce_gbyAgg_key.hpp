/*============================================================================*/
/* XF_Optimise_Reduce_gbyAgg_key.hpp -- Reduce key of gbyAgg                           */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Optimise_Reduce_gbyAgg_key : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    XF_Optimise_Reduce_gbyAgg_key(CardIter* pCI);
   ~XF_Optimise_Reduce_gbyAgg_key(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);

  };

  XF_Optimise_Reduce_gbyAgg_key* _pXF_Optimise_Reduce_gbyAgg_key(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
