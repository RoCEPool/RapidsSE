/*============================================================================*/
/* XF_Prolog_Analyse_emitted.hpp -- Analyse Emitted                           */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Analyse_emitted : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Analyse_emitted(CardIter* pCI);
   ~XF_Prolog_Analyse_emitted(void) noexcept(false);

    static void vAnalyse(CardIter* pCI);
    static void vXF_projection_Analysis_passThru(CardIter* pCI);

    void vXF_projection_Analysis(void);
    void vXF_projection_Analysis_passThru(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Analyse_emitted* _pXF_Prolog_Analyse_emitted(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
