/*============================================================================*/
/* XF_projection_Analysis.hpp -- Projection Prior                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_projection_Analysis : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_projection_Analysis(CardIter* pCI);
   ~XF_projection_Analysis(void) noexcept(false);

    static void vXF_projection_Analysis(CardIter* pCI);
    static void vXF_projection_Analysis_passThru(CardIter* pCI);

    void vXF_projection_Analysis(void);
    void vXF_projection_Analysis_passThru(void);

  private:
    Footer* aFooter(void);
  };

  XF_projection_Analysis* _pXF_projection_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
