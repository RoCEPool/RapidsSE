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
  class Card_BoolCalc;

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
    void vConvert_CrlPrmOdn_Into_StmCol(CardIter* pCI, Card_BoolCalc* p_boolCalc);
    void vConvert_SubQryCall_Into_StmCol(CardIter* pCI, Card_BoolCalc* p_boolCalc);
    void vMove_corrPred_To_table_filter(CardIter* pCI, Card_BoolCalc* p_boolCalc);

  private:
    Footer* aFooter(void);
  };

  CorrelatedJoin* _pCorrelatedJoin(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
