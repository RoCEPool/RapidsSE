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
    bool bSnap_IQ_crlKey(CardIter* pCI);
    void vPromote_IQ_aggreg_To_gbyAgg_crlKey(CardIter* pCI);
    void vEnhance_IQ_emit_project_Add_crlKey(CardIter* pCI);
    void vConvert_OQ_sqyNo_Into_expCol(CardIter* pCI, Card_Predicate* p_predicate);
    void vConvert_OQ_sqyPrm_Into_expCol(CardIter* pCI, Card_Predicate* p_predicate);
    void vMove_OQ_rlnConj_rslTest_To_from_filter(CardIter* pCI, Card_Predicate* p_predicate);
    void vRestate_emit_fold(CardIter* pCI);
    void vPromote_aggreg_To_gbyAgg(CardIter* pCI);

    Footer* aFooter(void);
  };

  XF_Flatten_24_W_A_C_ro* _pXF_Flatten_24_W_A_C_ro(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
