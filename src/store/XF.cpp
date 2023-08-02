/*============================================================================*/
/* XF.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "XF.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "XF_Add_level.hpp"
#include "XF_Cull_NonUsed_bound.hpp"
#include "XF_Cull_Used_bound.hpp"
#include "XF_Cut_sort.hpp"
#include "XF_Dedupe_as.hpp"
#include "XF_Delete_Unneeded.hpp"
#include "XF_Delete_flat.hpp"
#include "XF_Derive_projection_key.hpp"
#include "XF_Early_projection.hpp"
#include "XF_Flatten.hpp"
#include "XF_Individuate_filter.hpp"
#include "XF_Synthesise_Code.hpp"
#include "XF_Morph_scan_To_indexScan.hpp"
#include "XF_Morph_table_To_EEop.hpp"
#include "XF_Move_on.hpp"
#include "XF_Promote_gbyAgg_To_ordAgg.hpp"
#include "XF_Reduce_key_gbyAgg.hpp"
#include "XF_Reinstate_stage.hpp"
#include "XF_Shorten_queryNm.hpp"
#include "XF_as_Review.hpp"
#include "XF_index_Analysis.hpp"
#include "XF_not_Ops.hpp"
#include "XF_projection_Analysis.hpp"
#include "XF_rpnCalc_To_boolCalc.hpp"
#include "XF_scan_To_table.hpp"
#include "XF_stage_cast.hpp"
#include "XF_stmCol_To_colNm.hpp"
#include "XF_subQuery_Analysis.hpp"
#include "XF_table_Analysis.hpp"
#include "XF_tblCol_To_stmCol.hpp"
#include "XF_Column_Mapping.hpp"
#include "XF_colNm_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF, CN_XF)
__aNewFunction(XF, CN_XF)
__vDeleteFunction(XF, CN_XF)
__aFooterFunction(XF)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF::XF(CardIter* pCI)
: XF_TRANSFORM(CN_XF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  m_pCI = pCI;
  m_cTableSyn = 'a';

  m_xSymbol_tableSyn.vSetTableSyn('a');

  _checkInitialised_(CN_XF);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF::~XF(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF::vXF(CardIter* pCI)
{
  _static_

  XF* pXF = new XF(pCI);

  if (! AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU)) {
    pXF->vTransform();
  }
  else {
    pXF->vTransform_passThru();
  }

  Bench::vValidate(pCI);

  delete pXF;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply transformations                                                      */
/*----------------------------------------------------------------------------*/
void XF::vTransform(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard())
    XF_Shorten_queryNm::vXF_Shorten_queryNm(m_pCI);


  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    //vGroup_InitialAdjustments();
    XF_Move_on::vXF_Move_on(m_pCI);

    //vGroup_CondenseInfo();
    XF_Delete_Unneeded::vXF_Delete_Unneeded(m_pCI);
    XF_scan_To_table::vXF_scan_To_table(m_pCI);
    XF_Add_level::vXF_Add_level(m_pCI);
    XF_not_Ops::vXF_not_Ops(m_pCI);

    //vGroup_MetaAnalysis();
    XF_table_Analysis::vXF_table_Analysis(m_pCI,&m_cTableSyn,&m_xSymbol_tableSyn);
    XF_projection_Analysis::vXF_projection_Analysis(m_pCI);
    XF_index_Analysis::vXF_index_Analysis(m_pCI);

    //vGroup_PreliminaryConversions();
    XF_rpnCalc_To_boolCalc::vXF_rpnCalc_To_boolCalc(m_pCI);
    XF_as_Review::vXF_as_Review(m_pCI);

    vDebugPoint();
  }

  XF_subQuery_Analysis::vXF_subQuery_Analysis(m_pCI);

  m_pCI->m_pBench->vMadeSimpler(true);


  Bench::vValidate(m_pCI);


  XF_Flatten::vFlatten(m_pCI);
  Bench::vValidate(m_pCI);


  XF_Delete_flat::vXF_Delete_flat(m_pCI);

  XF_Morph_table_To_EEop::vXF_Morph_table_To_EEop(m_pCI);

  XF_Morph_scan_To_indexScan::vXF_Morph_scan_To_indexScan(m_pCI);

  XF_Derive_projection_key::vXF_Derive_projection_key(m_pCI);

  XF_Cut_sort::vXF_Cut_sort(m_pCI);

  XF_Promote_gbyAgg_To_ordAgg::vXF_Promote_gbyAgg_To_ordAgg(m_pCI);

  XF_Cull_NonUsed_bound::vXF_Cull_NonUsed_bound(m_pCI);

  XF_Cull_Used_bound::vXF_Cull_Used_bound(m_pCI);

  XF_Early_projection::vXF_Early_projection(m_pCI);

  XF_Individuate_filter::vXF_Individuate_filter(m_pCI);

  XF_Reduce_key_gbyAgg::vXF_Reduce_key_gbyAgg(m_pCI);
  m_pCI->m_pBench->vMadeOptimal(true);




  XF_Reinstate_stage::vXF_Reinstate_stage(m_pCI);

  XF_stage_cast::vXF_stage_cast(m_pCI);

  XF_tblCol_To_stmCol::vXF_tblCol_To_stmCol(m_pCI);

  XF_stmCol_To_colNm::vXF_stmCol_To_colNm(m_pCI);

  //XF_Dedupe_as::vXF_Dedupe_as(m_pCI);

  XF_Column_Mapping::vXF_Column_Mapping(m_pCI);

  m_pCI->m_pBench->vMadeWeavable(true);


  XF_Synthesise_Code::vXF_Make_Code(m_pCI);

}

/*----------------------------------------------------------------------------*/
/* apply transformations                                                      */
/*----------------------------------------------------------------------------*/
void XF::vTransform_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard())
    XF_Shorten_queryNm::vXF_Shorten_queryNm(m_pCI);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    XF_table_Analysis::vXF_table_Analysis_passThru(m_pCI,&m_cTableSyn,&m_xSymbol_tableSyn);
    XF_projection_Analysis::vXF_projection_Analysis_passThru(m_pCI);

    m_pCI->vTransform(__func__, XF_STMCOL_TO_TBLCOL, "all $n become $c.n"_Stz);
    while (m_pCI->G_bEachMain())
      m_pCI->G.p_OP->vConvert_stmColOdn_To_tblColOdn(m_pCI);
    m_pCI->vEndTransform();

    // todo: needs discussion with Alka re constraint
    XF_colNm_To_stmCol::vXF_colNm_To_stmCol(m_pCI);


    m_pCI->vTransform(__func__, XF_NONE, "adjust sort"_Stz);
    while (m_pCI->G_bEachMainWith2Sub(CK_SORT,CK_KEY,CK_PROJECTION)) {
      m_pCI->G.p_projection->vAdjust_Wrt_key(m_pCI->G.p_key);
    }
    m_pCI->vEndTransform();

    m_pCI->vTransform(__func__, XF_NONE, "adjust top"_Stz);
    while (m_pCI->G_bEachMainWith2Sub(CK_TOP,CK_KEY,CK_PROJECTION)) {
      m_pCI->G.p_projection->vAdjust_Wrt_key(m_pCI->G.p_key);
    }
    m_pCI->vEndTransform();



    m_pCI->vTransform(__func__, XF_NONE, "tweak index join"_Stz);
    while (m_pCI->G_bEachMain(CK_INDEXJOIN)) {
      m_pCI->G.p_indexJoin->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_indexJoin->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();
    m_pCI->vTransform(__func__, XF_NONE, "tweak index scan"_Stz);
    while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
      m_pCI->G.p_indexScan->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_indexScan->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();
    m_pCI->vTransform(__func__, XF_NONE, "tweak key exists"_Stz);
    while (m_pCI->G_bEachMain(CK_KEYEXISTS)) {
      m_pCI->G.p_keyExists->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_keyExists->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();
    m_pCI->vTransform(__func__, XF_NONE, "tweak key not exists"_Stz);
    while (m_pCI->G_bEachMain(CK_KEYNOTEXISTS)) {
      m_pCI->G.p_keyNotExists->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_keyNotExists->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();

    XF_rpnCalc_To_boolCalc::vXF_rpnCalc_To_boolCalc(m_pCI);

    vDebugPoint();
  }

  m_pCI->m_pBench->vMadeSimpler(true);

  Bench::vValidate(m_pCI);

  m_pCI->vUsingBoard((i8)0);
  XF_tblCol_To_stmCol::vXF_tblCol_To_stmCol(m_pCI);

  XF_Column_Mapping::vXF_Column_Mapping(m_pCI);

  XF_Dedupe_as::vXF_Dedupe_as(m_pCI);

  XF_Synthesise_Code::vXF_Make_Code(m_pCI);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
