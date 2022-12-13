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

    Card_Index*      C_pProduce_index_Of_table(Table* pTable);
    Card_IndexList*  C_pProduce_indexList_Of_table(Table* pTable);
    Card_Size*       C_pProduce_size_Of_table(Table* pTable);
    Card_Stream*     C_pProduce_stream_Of_aggreg(Card_AggElist* p_aggElist);
    Card_Stream*     C_pProduce_stream_Of_project(Card_ProjElist* p_projElist);
    Card_Stream*     C_pProduce_stream_Of_table(Table* pTable);
    Card_TablePtn*   C_pProduce_tablePtn_Of_table(Card_Table* p_table);

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
