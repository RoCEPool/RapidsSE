/*============================================================================*/
/* Xcreate.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xverify.hpp"

namespace STORE
{
  class Table;

  class Xcreate : public Xverify
  {
  public:

    Card_Bound*      C_pBound_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                 char cTblSyn);

    Card_Bound*      C_pBound_CreateFrom_PRED(Card_PRED* p_PRED,
                                               char cTblSyn);

    Card_Bound*      C_pBound_New(Stz* szRelnOp);

    Card_BoolCalc*   C_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc);

    Card_Bounds*     C_pBounds_CreateFrom_PRED(Card_PRED* p_PRED,
                                               char cTblSyn);

    Card_Bounds*     C_pBounds_Create_Eq_ForGivenCol(Stz* szTblColOdn);
    Card_Bounds*     C_pBounds_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                  char cTblSyn);


    void             C_vBounds_DeriveFrom_filter(Card_Table* p_pred_Of_filter,
                                                 Card_Filter* p_filter,
                                                 bool bParamAllowedInBounds);


    Card_StmColList* C_pStmColList_DeriveFrom_ELIST(Card_ELIST* p_ELIST);

    Card_Filter*     C_pFilter_From_PRED(Card_PRED* p_PRED);

    // must be last
    Footer m_xFooter;

  public:
    Xcreate(eCN eClass);
   ~Xcreate(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
