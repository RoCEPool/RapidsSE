/*============================================================================*/
/* XF_Prolog_Analyse_index.hpp -- Index Prior                                    */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Analyse_index : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Analyse_index(CardIter* pCI);
   ~XF_Prolog_Analyse_index(void) noexcept(false);

    static void vAnalyse(CardIter* pCI);
    static void vXF_index_Analysis_passThru(CardIter* pCI);

    void vXF_Analyse_index(void);
    void vXF_Analyse_index_passThru(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Analyse_index* _pXF_Prolog_Analyse_index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
