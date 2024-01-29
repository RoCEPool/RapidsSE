/*============================================================================*/
/* CorrelatedJoin.hpp -- CorrelatedJoin                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eSM.h"

namespace STORE
{
  class CardIter;
  class CarryForward;
  class Card_BoolPred;

  class CorrelatedJoin : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CorrelatedJoin(void);
   ~CorrelatedJoin(void) noexcept(false);

    void vCorrelate(CardIter* pCI, eSM eCorrelate);

    void vVia_gbyAgg_Promotion(CardIter* pCI);

  private:

    bool bIdentify_Correlation_Key_Predicate(CardIter* pCI);
    void vIdentify_Expr_Of_Correlation_Key_Column(CardIter* pCI);
    void vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Key(CardIter* pCI);
    void vEnhance_sq_project_To_Include_Correlation_Key(CardIter* pCI, Stz* szAs);
    void vConvert_SqyNm_Into_ExpCol(CardIter* pCI, Card_BoolPred* p_boolPred);
    void vConvert_SqyPrm_Into_ExpCol(CardIter* pCI, Card_BoolPred* p_boolPred);
    void vMove_corrPred_To_table_filter(CardIter* pCI, Card_BoolPred* p_boolPred);

  private:
    Footer* aFooter(void);
  };

  CorrelatedJoin* _pCorrelatedJoin(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
