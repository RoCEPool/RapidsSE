/*============================================================================*/
/* XF_Prolog_Analyse_emit.hpp -- Analyse Emit                                 */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Analyse_emit : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Analyse_emit(CardIter* pCI);
   ~XF_Prolog_Analyse_emit(void) noexcept(false);

    static void vAnalyse(CardIter* pCI);

    void vXF_Analyse(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Analyse_emit* _pXF_Prolog_Analyse_emit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
