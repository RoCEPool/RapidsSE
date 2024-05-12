/*============================================================================*/
/* XF_FLATTEN.hpp -- FLATTEN                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_FLATTEN : public XF_TRANSFORM
  {
  public:

    XF_FLATTEN(eCN eClass, CardIter* pCI);
   ~XF_FLATTEN(void) noexcept(false);

  protected:
    void vFold_from(Stz* szFoldNm, CardIter* pCI);
    void vFold_subQuery(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ);

    void vInto_KEYnotEXISTS(CardIter* pCI);

    void vSet_smooth(CardIter* pCI, eCK eCardWithSQ, eQO eWhatOp);
    void vSmooth(CardIter* pCI);

    static i8 nTables_In_SubQuery(CardIter* pCI, Stz* szFold, eCK eCardWithSQ);

    void vRotate_Next(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool bSnap_IQ_crlKey(CardIter* pCI);
    void vPromote_IQ_aggreg_To_gbyAgg_crlKey(CardIter* pCI);
    void vEnhance_IQ_emit_project_Add_crlKey(CardIter* pCI);
    void vConvert_OQ_sqyNo_Into_expCol(CardIter* pCI, Card_Predicate* p_predicate);
    void vConvert_OQ_sqyPrm_Into_expCol(CardIter* pCI, Card_Predicate* p_predicate);
    void vMove_OQ_rlnConj_rslTest_To_from_filter(CardIter* pCI, Card_Predicate* p_predicate);
    void vRestate_emit_fold(CardIter* pCI);
    void vPromote_aggreg_To_gbyAgg(CardIter* pCI);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
