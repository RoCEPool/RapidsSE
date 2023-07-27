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
void Smooth::vSmooth(CardIter* pCI, CarryForward* pCarryForward, bool bEarlyExit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SMOOTH, _FOOTER_);

  // todo: REMOVE
  if (bEarlyExit)
    return;


  pCI->vRestart();
  RASSERT(pCI->G_bEachMain(CK_FOLD));
  RASSERT(pCI->G.p_MainCard->bIsCard(CK_FOLD));
  //RASSERT(pCI->m_pExpr_fold);

  // Table*
  while (pCI->G_bIsNextMain(CK_TABLE) && pCI->G_bEachMain(CK_TABLE)) {
    if (pCarryForward->Table.m_bSeen) {
      pCI->G.p_table->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);
    }
  }



  // [Where]
  if (pCI->G_bIsNextMain(CK_WHERE) && pCI->G_bEachMain(CK_WHERE)) {
    if (pCarryForward->Where_preAgg.m_bSeen) {
      pCI->G.p_where->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);
    }
  }


  // GbyAgg_partition
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);
    }
  }

  // GbyAgg_node
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);
    }
  }

  // GbyAgg_origin
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);

    }
    pCI->Analysis.m_pProjection_prior = pCI->G.p_gbyAgg->pSub_projection();
  }

  // [project (avg)]
  if (pCI->G_bIsNextMain(CK_PROJECT) && pCI->G_bEachMain(CK_PROJECT)) {
    if (pCarryForward->Project_postAgg.m_bSeen) {
      pCI->G.p_project->vSmooth_postAgg(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);
    }
  }

  // Having
  if (pCI->G_bIsNextMain(CK_HAVING) && pCI->G_bEachMain(CK_HAVING)) {
    if (pCarryForward->Having.m_bSeen) {
      pCI->G.p_having->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);
    }
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --  --

  if (pCarryForward->Project_postAgg.m_bSeen &&  pCI->G.p_having) {
    pCI->G.p_project->vInject_foldExpr(pCI);
    pCI->G.p_having->vAcclimate_foldExpr(pCI);
    pCI->G.p_projection = pCI->G.p_project->pSub_projection();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
  }
  else
  if (pCarryForward->Unproject.m_bUnproject_foldExpr) {
    pCI->G.p_projection = pCI->G.p_gbyAgg->pSub_projection();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  pCI->vPrintBoard(__LINE__);
  pCI->vRenumberCards();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
