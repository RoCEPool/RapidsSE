/*============================================================================*/
/* Xreshape.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xgearing.hpp"


namespace STORE
{
  class Table;

  class Xreshape : public Xgearing
  {
  public:

    // utilities
    void R_vUTE_Resolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(Card_BoolCalc* p_boolCalc);
    i8   R_iUTE_Stage_Partitioning(Card* p_main_prior,
                                   Card_Combined* p_combined_prior,
                                   Card_Next* p_next_prior,
                                   Card* p_main_curr);
    void R_vUTE_Stream_AdjustLoHi(void);
    void R_vUTE_Unproject_foldExpr_After_having(void);
    void R_vUTE_Unproject_foldExpr_After_where(void);

    // vSeries_26_SubQuery_Analysis
    void vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc(void);
    void vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc(void);

    // vSeries_33_To_TblColOdn
    void vR_S33_T10_Convert_stmColOdn_To_tblColOdn(void);

    // vSeries_75_EE_operators
    void vR_S75_T10_Morph_table_To_EEop_v2(void);
    void vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void);

    // vSeries_Z0_Restate_Pipeline
    void vR_SZ0_T20_Reinstate_stagesV3(void);

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xreshape(eCN eClass, Bench* pBench);
   ~Xreshape(void) noexcept(false);

  private:


    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
