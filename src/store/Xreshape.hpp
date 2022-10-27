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

    void vR_H10_T10_Delete_Redundant_stage(void);
    void vR_H10_T15_Hide_Ignorable_stage(void);
    void vR_H10_T20_Delete_absJoin(void);
    void vR_H10_T30_Delete_combined(void);
    void vR_H10_T40_Delete_send(void);
    void vR_H10_T50_Cull_scan_Leaving_table(void);
    void vR_H20_T10_Add_tableSyn_ForAll_table(void);
    void vR_H20_T20_Add_tablePtn_UnderAll_table(void);
    void vR_H30_T10_Add_stream_UnderAll_table(void);
    void vR_H30_T20_Calc_ordinalRange_ForAll_stream(void);
    void vR_H40_T10_Convert_where_rpnCalc_To_calcBool(void);
    void vR_H40_T20_Mark_where_pred_Wrt_table(void);
    void vR_H40_T30_Move_Marked_where_pred_To_table_filter(void);
    void vR_H40_T40_Delete_Empty_where(void);
    void vR_H50_T10_Rework_Exists_SubQueries(void);
    void vR_H50_T20_Copy_Up_SubQueries(void);
    void vR_H50_T30_Resolve_Parameter_Linkage(void);
    void vR_H50_T40_Resolve_Parameter_Terms(void);
    void vR_H60_T10_Add_indexJoin_UnderFiltered_table(void);
    void vR_H60_T20_Convert_idxCol_name_Of_table_To_ordinal(void);
    void vR_H60_T30_Add_joinPred_UnderAll_indexJoin(void);
    void vR_H60_T40_Cull_filter_pred_MovedUpTo_joinPred(void);
    void vR_H60_T50_Delete_Empty_filter(void);
    void vR_H70_T00_Extract_tableList_From_queryBoard(void);
    void vR_H70_T10_Extract_joinPredList(void);
    void vR_H70_T20_Compute_coPtnPredList(void);
    void vR_H70_T30_Compute_coPtnJoinList(void);
    void vR_H70_T40_Compute_crossPtnJoinList(void);
    void vR_H70_T50_Compute_ptndJoinSet(void);
    void vR_H70_T60_Copy_ptndJoinSet(void);
    void vR_H70_T70_Generate_ptndPermutations(void);
    void vR_H70_T80_Remove_invalidPermutations(void);
    void vR_H70_T90_Setup_costEst(void);
    void vR_H70_T92_Estimate_ptndJoins(void);
    void vR_H70_T94_Estimate_network(void);
    void vR_H70_T96_Estimate_joinOrder(void);
    void vR_H70_T98_Extract_joinOrderSeq(void);
    void vR_H70_T99_Convert_table_To_scan_Or_indexJoin(void);
    void vR_H70_T99_Convert_table_To_scan_Or_indexJoin_b(void);
    void vR_H70_T99_Convert_table_To_scan_Or_indexJoin_c(void);
    void vR_H80_T10_Insert_indexList_ToAll_scan(void);
    void vR_H80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord(void);
    void vR_H80_T30_Mark_scan_pred_bounds(void);
    void vR_H80_T40_Mark_scan_filter_term_WhichMatchIndex(void);
    void vR_H80_T50_Add_scanPred(void);
    void vR_H80_T60_Cull_Marked_cmpPred(void);
    void vR_H80_T70_Promote_scan_To_indexScan(void);
    void vR_H80_T70_Promote_scan_To_indexScan_b(void);
    void vR_H80_T70_Promote_scan_To_indexScan_c(void);
    void vR_H90_T10_Remove_sort_IfSameAs_gby(void);
    void vR_HA0_T10_Individuate_filter(void);
    void vR_HA0_T10_Individuate_filter_b(void);
    void vR_HZ0_T10_Reinstate_stages(void);
    void vR_HZ0_T20_Reinstate_combined(void);

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
