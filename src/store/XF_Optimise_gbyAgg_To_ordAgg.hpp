/*============================================================================*/
/* XF_OT_Replace_gbyAgg_By_ordAgg.hpp -- Promote gbyAgg to ordAgg                */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_OT_Replace_gbyAgg_By_ordAgg : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_OT_Replace_gbyAgg_By_ordAgg(CardIter* pCI);
   ~XF_OT_Replace_gbyAgg_By_ordAgg(void) noexcept(false);

    static void vXF_Promote_gbyAgg_To_ordAgg(CardIter* pCI);

    void vXF_Promote_gbyAgg_To_ordAgg(void);

  private:
    Footer* aFooter(void);
  };

  XF_OT_Replace_gbyAgg_By_ordAgg* _pXF_OT_Replace_gbyAgg_By_ordAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
