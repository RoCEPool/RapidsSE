/*============================================================================*/
/* XF_projection_Analysis.hpp -- Projection Prior                          */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_projection_Analysis : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_projection_Analysis(CardIter* pCI);
   ~XF_Prolog_projection_Analysis(void) noexcept(false);

    static void vProlog(CardIter* pCI);
    static void vXF_projection_Analysis_passThru(CardIter* pCI);

    void vXF_projection_Analysis(void);
    void vXF_projection_Analysis_passThru(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_projection_Analysis* _pXF_Prolog_projection_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
