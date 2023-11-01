/*============================================================================*/
/* XF_Prolog_Analyse_injected.hpp -- Analyse Injected                         */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Analyse_injected : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Analyse_injected(CardIter* pCI);
   ~XF_Prolog_Analyse_injected(void) noexcept(false);

    static void vAnalyse(CardIter* pCI);
    static void vAnalyse_injected_passThru(CardIter* pCI);

    void vAnalyse_injected(void);
    void vAnalyse_injected_passThru(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Analyse_injected* _pXF_Prolog_Analyse_injected(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
