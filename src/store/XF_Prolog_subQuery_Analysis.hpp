/*============================================================================*/
/* XF_Prolog_subQuery_Analysis.hpp -- SubQuery Prior                              */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_subQuery_Analysis : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_subQuery_Analysis(CardIter* pCI);
   ~XF_Prolog_subQuery_Analysis(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vXF_subQuery_Analysis(void);

  private:
    void vAnalysis_Of_from_call(CardIter* pCI);
    void vAnalysis_Of_having_boolCalc(CardIter* pCI);
    void vAnalysis_Of_where_boolCalc(CardIter*  pCI);

    Footer* aFooter(void);
  };

  XF_Prolog_subQuery_Analysis* _pXF_Prolog_subQuery_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
