/*============================================================================*/
/* XF.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "XF.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "XF_Flatten.hpp"

#include "XF_Add_level.hpp"
#include "XF_as_Review.hpp"
#include "XF_Delete_Unneeded.hpp"
#include "XF_index_Analysis.hpp"
#include "XF_Move_on.hpp"
#include "XF_not_Ops.hpp"
#include "XF_projection_Analysis.hpp"
#include "XF_rpnCalc_To_boolCalc.hpp"
#include "XF_Shorten_queryNm.hpp"
#include "XF_scan_To_table.hpp"
#include "XF_subQuery_Analysis.hpp"
#include "XF_table_Analysis.hpp"
#include "XF_tblCol_To_stmCol.hpp"


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

  pXF->vTransform();

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

  if (! AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU)) {

    vSimplify();
    Bench::vValidate(m_pCI);


    XF_Flatten::vFlatten(m_pCI);
    Bench::vValidate(m_pCI);

  }
  else {

    vSimplify_passThru();

    Bench::vValidate(m_pCI);

    XF_tblCol_To_stmCol::vXF_tblCol_To_stmCol(m_pCI);


  }
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void XF::vSimplify(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);
    XF_Shorten_queryNm::vXF_Shorten_queryNm(m_pCI);
  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);

    //vGroup_InitialAdjustments();
     XF_Move_on::vXF_Move_on(m_pCI);

    //vGroup_CondenseInfo();
    XF_Delete_Unneeded::vXF_Delete_Unneeded(m_pCI);
    XF_scan_To_table::vXF_scan_To_table(m_pCI);
    XF_Add_level::vXF_Add_level(m_pCI);
    XF_not_Ops::vXF_not_Ops(m_pCI);

    //vGroup_MetaAnalysis();
    XF_table_Analysis::vXF_table_Analysis(m_pCI,&m_cTableSyn);
    XF_projection_Analysis::vXF_projection_Analysis(m_pCI);
    XF_index_Analysis::vXF_index_Analysis(m_pCI);

    //vGroup_PreliminaryConversions();
    XF_rpnCalc_To_boolCalc::vXF_rpnCalc_To_boolCalc(m_pCI);
    XF_as_Review::vXF_as_Review(m_pCI);

    vDebugPoint();
  }

  XF_subQuery_Analysis::vXF_subQuery_Analysis(m_pCI);

  m_pCI->m_pBench->vMadeSimpler(true);

}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void XF::vSimplify_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);
    XF_Shorten_queryNm::vXF_Shorten_queryNm(m_pCI);
  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);

    XF_table_Analysis::vXF_table_Analysis_passThru(m_pCI,&m_cTableSyn);
    XF_projection_Analysis::vXF_projection_Analysis_passThru(m_pCI);

    XF_projection_Analysis::vXF_projection_Analysis_passThru(m_pCI);

    m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

    while (m_pCI->G_bEachMain())
      ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_stmColOdn_To_tblColOdn(m_pCI);

    m_pCI->vEndTransform();

    m_pCI->vTransform(__func__, "all colNm become $c.n"_Stz);

    while (m_pCI->G_bEachMain())
      ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_colNm_To_stmCol(m_pCI);

    m_pCI->vEndTransform();

    vDebugPoint();
  }

  m_pCI->m_pBench->vMadeSimpler(true);


}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
