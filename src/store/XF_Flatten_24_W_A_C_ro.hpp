/*============================================================================*/
/* XF_Flatten_24_W_A_C_ro.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"

namespace STORE
{
  class XF_Flatten_24_W_A_C_ro : public XF_FLATTEN
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_24_W_A_C_ro(CardIter* pCI);
   ~XF_Flatten_24_W_A_C_ro(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vFlatten(void);

  private:
    bool bSnap_term_sqyPrm(CardIter* pCI);
    void vIdentify_Expr_Of_Correlation_Key_Column(CardIter* pCI);
    void vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Key(CardIter* pCI);
    void vEnhance_sq_project_To_Include_Correlation_Key(CardIter* pCI, Stz* szAs);
    void vConvert_SqyNo_Into_ExpCol(CardIter* pCI, Card_BoolPred* p_boolPred);
    void vConvert_SqyPrm_Into_ExpCol(CardIter* pCI, Card_BoolPred* p_boolPred);
    void vMove_corrPred_To_table_filter(CardIter* pCI, Card_BoolPred* p_boolPred);
    void vRestate_emit_fold(CardIter* pCI);
    void vVia_gbyAgg_Promotion(CardIter* pCI);

    Footer* aFooter(void);
  };

  XF_Flatten_24_W_A_C_ro* _pXF_Flatten_24_W_A_C_ro(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
