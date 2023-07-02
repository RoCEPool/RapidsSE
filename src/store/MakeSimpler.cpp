/*============================================================================*/
/* MakeSimpler.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "MakeSimpler.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"
#include "OuterQuery.hpp"

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

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeSimpler, CN_MAKESIMPLER)
__aNewFunction(MakeSimpler, CN_MAKESIMPLER)
__vDeleteFunction(MakeSimpler, CN_MAKESIMPLER)
__aFooterFunction(MakeSimpler)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeSimpler::MakeSimpler(CardIter* pCI)
: Object(CN_MAKESIMPLER)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI = pCI;
  m_cTableSyn = 'a';

  _checkInitialised_(CN_MAKESIMPLER);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeSimpler::~MakeSimpler(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vSimplify(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

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
    vTableAnalysis(&m_cTableSyn);
    XF_projection_Analysis::vXF_projection_Analysis(m_pCI);
    XF_index_Analysis::vXF_index_Analysis(m_pCI);

    //vGroup_PreliminaryConversions();
    XF_rpnCalc_To_boolCalc::vXF_rpnCalc_To_boolCalc(m_pCI);
    XF_as_Review::vXF_as_Review(m_pCI);

    vDebugPoint();
  }

  OuterQuery* pOuterQuery = new OuterQuery;
  pOuterQuery->vQueryAnalysis(m_pCI);
  delete pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vSimplify_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);
    XF_Shorten_queryNm::vXF_Shorten_queryNm(m_pCI);
  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);

    vTableAnalysis_passThru(&m_cTableSyn);
    XF_projection_Analysis::vXF_projection_Analysis_passThru(m_pCI);

    XF_projection_Analysis::vXF_projection_Analysis_passThru(m_pCI);

    m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

    while (m_pCI->G_bEachMain())
      ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_stmColOdn_To_tblColOdn(m_pCI);

    m_pCI->vEndTransform();

    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vTableAnalysis(char* acTableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn(m_pCI, acTableSyn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
  m_pCI->G_vRestart();
  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vTableAnalysis_passThru(char* acTableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn_passThru(m_pCI, acTableSyn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
//  m_pCI->G_vRestart();
//  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
