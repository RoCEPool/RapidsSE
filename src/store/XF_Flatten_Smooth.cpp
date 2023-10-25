/*============================================================================*/
/* XF_Flatten_Smooth.cpp -- XF_Flatten_Smooth                                                       */
/*============================================================================*/

#include "XF_Flatten_Smooth.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Smooth, CN_XF_FLATTEN_SMOOTH)
__aNewFunction(XF_Flatten_Smooth, CN_XF_FLATTEN_SMOOTH)
__vDeleteFunction(XF_Flatten_Smooth, CN_XF_FLATTEN_SMOOTH)
__aFooterFunction(XF_Flatten_Smooth)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Smooth::XF_Flatten_Smooth(void)
: Object(CN_XF_FLATTEN_SMOOTH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOTH, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_SMOOTH);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOTH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Smooth::~XF_Flatten_Smooth(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOTH, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Smooth::vSmooth(CardIter* pCI, CarryForward* pCarryForward, bool bEarlyExit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOTH, _FOOTER_);

  // todo: REMOVE
  if (bEarlyExit)
    return;


  pCI->vPrintBoard(__LINE__);

  pCI->vBeginEach();
  RASSERT(pCI->G_bEachMain(CK_FOLD));
  RASSERT(pCI->G.p_MainCard->bIsCard(CK_FOLD));
  //RASSERT(pCI->m_pExpr_fold);

  if(pCI->G_bIsNextMain(CK_SUBQUERY))
    RASSERT(pCI->pAdvanceChild());

  // Table*
  while (pCI->G_bIsNextMain(CK_FROM) && pCI->G_bEachMain(CK_FROM)) {
    if (pCarryForward->Table.m_bSeen) {
      pCI->G.p_from->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__LINE__);
    }
  }



  // [Where]
  if (pCI->G_bIsNextMain(CK_WHERE) && pCI->G_bEachMain(CK_WHERE)) {
    if (pCarryForward->Where_preAgg.m_bSeen) {
      //pCI->Flux.m_pArriving->vDump(__func__,"Smooth where"_Stz);
      pCI->G.p_where->vSmooth(pCI, pCarryForward);
     // pCI->vPrintBoard(__LINE__);
    }
  }


  // GbyAgg_partition
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
     // pCI->vPrintBoard(__LINE__);
    }
  }

  // GbyAgg_node
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
     // pCI->vPrintBoard(__LINE__);
    }
  }

  // GbyAgg_origin
  if (pCI->G_bIsNextMain(CK_GBYAGG) && pCI->G_bEachMain(CK_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
     // pCI->vPrintBoard(__LINE__);

    }
    pCI->Flux.m_pArriving = pCI->G.p_gbyAgg->pSub_emit();
  }

  // [project (avg)]
  if (pCI->G_bIsNextMain(CK_PROJECT) && pCI->G_bEachMain(CK_PROJECT)) {
    if (pCarryForward->Project_postAgg.m_bSeen) {
      pCI->G.p_project->vSmooth_postAgg(pCI, pCarryForward);
     // pCI->vPrintBoard(__LINE__);
    }
  }

  // Having
  if (pCI->G_bIsNextMain(CK_HAVING) && pCI->G_bEachMain(CK_HAVING)) {
    if (pCarryForward->Having.m_bSeen) {
      pCI->G.p_having->vSmooth(pCI, pCarryForward);
     // pCI->vPrintBoard(__LINE__);
    }
  }

  // Sort
  if (pCI->G_bIsNextMain(CK_SORT) && pCI->G_bEachMain(CK_SORT)) {
    if (pCarryForward->Sort.m_bSeen) {
      pCI->G.p_sort->vSmooth(pCI, pCarryForward);
     // pCI->vPrintBoard(__LINE__);
    }
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --  --

  if (pCarryForward->Project_postAgg.m_bSeen &&  pCI->G.p_having) {
    pCI->G.p_project->vInject_foldExpr(pCI);
    pCI->G.p_having->vAcclimate_foldExpr(pCI);
    pCI->G.p_emit = pCI->G.p_project->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
   // pCI->vPrintBoard(__LINE__);
  }
  else
  if (pCarryForward->Unproject.m_bUnproject_foldExpr) {
    pCI->G.p_emit = pCI->G.p_gbyAgg->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
   // pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

 // pCI->vPrintBoard(__LINE__);
 // pCI->vRenumberCards();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
