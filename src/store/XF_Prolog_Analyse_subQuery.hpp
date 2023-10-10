/*============================================================================*/
/* XF_Prolog_Analyse_subQuery.hpp -- Analyse SubQuery                         */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Analyse_subQuery : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Analyse_subQuery(CardIter* pCI);
   ~XF_Prolog_Analyse_subQuery(void) noexcept(false);

    static void vAnalyse(CardIter* pCI);

    void vAnalyse_subQuery(void);

  private:
    void vAnalysis_Of_from_call(CardIter* pCI);
    void vAnalysis_Of_having_boolCalc(CardIter* pCI);
    void vAnalysis_Of_where_boolCalc(CardIter*  pCI);

    Footer* aFooter(void);
  };

  XF_Prolog_Analyse_subQuery* _pXF_Prolog_Analyse_subQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
