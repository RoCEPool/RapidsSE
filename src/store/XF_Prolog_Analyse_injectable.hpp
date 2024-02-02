/*============================================================================*/
/* XF_Prolog_Analyse_injectable.hpp -- Analyse Injected                         */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Analyse_injectable : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Analyse_injectable(CardIter* pCI);
   ~XF_Prolog_Analyse_injectable(void) noexcept(false);

    static void vAnalyse(CardIter* pCI);
    static void vAnalyse_injected_passThru(CardIter* pCI);

    void vAnalyse_injectable(void);
    void vAnalyse_injectable_passThru(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Analyse_injectable* _pXF_Prolog_Analyse_injectable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
