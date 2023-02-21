/*============================================================================*/
/* Smooth.cpp -- Smooth                                                       */
/*============================================================================*/

#include "Smooth.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Smooth, CN_SMOOTH)
__aNewFunction(Smooth, CN_SMOOTH)
__vDeleteFunction(Smooth, CN_SMOOTH)
__aFooterFunction(Smooth)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Smooth::Smooth(void)
: Object(CN_SMOOTH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SMOOTH, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SMOOTH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Smooth::~Smooth(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_SMOOTH, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Smooth::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SMOOTH, _FOOTER_);

  pCI->vRestart();
  RASSERT(pCI->G_bEachMain(CK_FOLD));
  RASSERT(pCI->G.p_MainCard->bIsCard(CK_FOLD));
  RASSERT(pCI->m_pExpr_fold);

  // Table*
  i8 nTables = 0;
  while (pCI->G_bIsNextMain(CK_TABLE) && pCI->G_bEachMain(CK_TABLE)) {
    nTables++;
    pCI->G.p_table->vSmooth(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
  }

  // [Where]
  bool bSawWhere = false;
  if (pCI->G_bIsNextMain(CK_WHERE) && pCI->G_bEachMain(CK_WHERE)) {
    bSawWhere = true;
    pCI->G.p_where->vSmooth(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  // GbyAgg_partition
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  // GbyAgg_node
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  // GbyAgg_origin
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  // [project (avg)]
  bool bSawProject_postAgg = false;
  if (pCI->G_bIsNextMain(CK_PROJECT) && pCI->G_bEachMain(CK_PROJECT)) {
    bSawProject_postAgg = true;
    pCI->G.p_project->vSmooth_postAgg(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  // Having
  if (pCI->G_bIsNextMain(CK_HAVING) && pCI->G_bEachMain(CK_HAVING)) {
    pCI->G.p_having->vSmooth(pCI, bSawProject_postAgg, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --  --

  if (pCarryForward->Project_postAgg.m_bSeen) {
    pCI->G.p_project->vInject_foldExpr(pCI);
    pCI->G.p_having->vAcclimate_foldExpr(pCI);
    pCI->G.p_projection = pCI->G.p_project->pSub_projection();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
  }
  else {
    pCI->G.p_projection = pCI->G.p_gbyAgg->pSub_projection();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
