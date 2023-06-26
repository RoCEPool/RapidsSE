/*============================================================================*/
/* XF_Reduce_key_gbyAgg.hpp -- Reduce key of gbyAgg                           */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Reduce_key_gbyAgg : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    XF_Reduce_key_gbyAgg(CardIter* pCI);
   ~XF_Reduce_key_gbyAgg(void) noexcept(false);

    static void vXF_Reduce_key_gbyAgg(CardIter* pCI);

    void vXF_Reduce_key_gbyAgg(void);

  private:
    Footer* aFooter(void);

  };

  XF_Reduce_key_gbyAgg* _pXF_Reduce_key_gbyAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
