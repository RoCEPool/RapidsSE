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
    bool bSnap_term_crlKey(CardIter* pCI);
    void vSnap_IQ_expr_crlKey(CardIter* pCI);
    void vPromote_IQ_aggreg_To_gbyAgg_crlKey(CardIter* pCI);
    void vEnhance_IQ_project_Add_crlKey(CardIter* pCI);
    void vConvert_sqyNo_Into_expCol(CardIter* pCI, Card_BoolPred* p_boolPred);
    void vConvert_SqyPrm_Into_ExpCol(CardIter* p_conj_arg, Card_BoolPred* p_boolPred);
    void vMove_rlnConj_sqyRslTest_To_from_filter(CardIter* pCI, Card_BoolPred* p_boolPred);
    void vRestate_emit_fold(CardIter* pCI);
    void vPromote_aggreg_To_gbyAgg(CardIter* pCI);

    Footer* aFooter(void);
  };

  XF_Flatten_24_W_A_C_ro* _pXF_Flatten_24_W_A_C_ro(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
