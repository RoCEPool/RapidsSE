/*============================================================================*/
/* XF.hpp -- Transformer                                                      */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

//#define Z_EARLY_PROJECTION_v2

namespace STORE
{
  class XF : public XF_TRANSFORM
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF(CardIter* pCI);
   ~XF(void) noexcept(false);

    static void vXF(CardIter* pCI);

    void vTransform(void);
    void vTransform_passThru(void);

  private:

    void vCrosscheck_Optimisations(void);

  protected:
    Footer* aFooter(void);
  };

  XF* _pXF(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
