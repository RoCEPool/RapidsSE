/*============================================================================*/
/* XF_Promote_gbyAgg_To_ordAgg.hpp -- Promote gbyAgg to ordAgg                */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Promote_gbyAgg_To_ordAgg : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Promote_gbyAgg_To_ordAgg(CardIter* pCI);
   ~XF_Promote_gbyAgg_To_ordAgg(void) noexcept(false);

    static void vXF_Promote_gbyAgg_To_ordAgg(CardIter* pCI);

    void vXF_Promote_gbyAgg_To_ordAgg(void);

  private:
    Footer* aFooter(void);
  };

  XF_Promote_gbyAgg_To_ordAgg* _pXF_Promote_gbyAgg_To_ordAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
