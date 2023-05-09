/*============================================================================*/
/* MakeWeavable.cpp -- Make Weavable                                          */
/*============================================================================*/

#include "MakeWeavable.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeWeavable, CN_MAKEWEAVABLE)
__aNewFunction(MakeWeavable, CN_MAKEWEAVABLE)
__vDeleteFunction(MakeWeavable, CN_MAKEWEAVABLE)
__aFooterFunction(MakeWeavable)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeWeavable::MakeWeavable(CardIter* pCI)
: Object(CN_MAKEWEAVABLE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_MAKEWEAVABLE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeWeavable::~MakeWeavable(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vWeavable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

//  vSeries(BK_SUBQUERY, 0, __func__);

  vCullNonusedBounds();
  vCullUsedBoundsFromFilter();

  vIndividuateFilter();

  vReinstateStages();

  vAddStageCasts();

  vTblColToStmCol();
  vStmColToColNmOfKeyExists();

//  vEndSeries(BK_SUBQUERY, 0);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add stage next casts                                                       */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vAddStageCasts(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI->vTransform(__func__, "add stage casts"_Stz);

  m_pCI->G.pDeck->vAddStageCasts(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vCullNonusedBounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMain()) {

    if (!(m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
          m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (!(m_pCI->G.p_indexList = m_pCI->G.p_MainCard->pSub_indexList()))
      continue;

    // have <bounds> & <filter> under <main>
    Card_Index::vCullNonusedBounds(m_pCI);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vCullUsedBoundsFromFilter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (! (m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) ||
           m_pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)
    ))
      continue;
    if (! (m_pCI->G.p_bounds = m_pCI->G.p_MainCard->pSub_bounds()))
      continue;

    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {

      // have <bounds> & <filter> under <main>

      CardIter xCR(m_pCI->G.p_filter);
      while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
        if (m_pCI->G.p_bounds->bHasBoundOf(m_pCI->G.p_cmpPred)) {
          //RASSERT(m_pCI->G.p_cmpPred->bIsJoinPred());
          m_pCI->G.p_cmpPred->vMark1(MARK_AT);
        }
      }

      continue;
    }

    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {

      // have <bounds> & <filter> under <main>

      CardIter xCR(m_pCI->G.p_constraint);
      while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
        if (m_pCI->G.p_bounds->bHasBoundOf(m_pCI->G.p_cmpPred)) {
          //RASSERT(m_pCI->G.p_cmpPred->bIsJoinPred());
          m_pCI->G.p_cmpPred->vMark1(MARK_AT);
        }
      }

      continue;
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->G_vRestart();
//  Card_Constraint::vCullUsedBounds(m_pCI);

  while (m_pCI->G_bEachMain()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      m_pCI->G.p_filter->vCullPred(MARK_AT);
    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint()))
      m_pCI->G.p_constraint->vCullPred(MARK_AT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->G_vRestart();
  Card_Filter::vDeleteEmptyFilters(m_pCI);

//  while (m_pCI->G_bEachMain()) {
//    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
//      m_pCI->G.p_MainCard->vDeleteFilterIfEmpty(m_pCI->G.p_filter);
//  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vIndividuateFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MAKEWEAVABLE,_FOOTER_);

  m_pCI->vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  m_pCI->G_vRestart();
  Card_Scan::vIndividuateFilter(m_pCI);

  m_pCI->G_vRestart();
  Card_IndexScan::vIndividuateFilter(m_pCI);

  m_pCI->G_vRestart();
  Card_IndexJoin::vIndividuateFilter(m_pCI);

  m_pCI->G_vRestart();
  Card_CrossJoin::vIndividuateFilter(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vReinstateStages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI->vTransform(__func__, "reinstate stages"_Stz);

  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_PROJECTION,0);
  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  m_pCI->vPrintBoard(__LINE__);

  Card_Stage::vReinstate_stages(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vStmColToColNmOfKeyExists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MAKEWEAVABLE,_FOOTER_);

  m_pCI->vTransform(__func__, "((20))))"_Stz);

  m_pCI->vRestart();

  while (m_pCI->G_bEachMain()) {

    if (! (   CK_KEYEXISTS    == m_pCI->G.e_MainCard
           || CK_KEYNOTEXISTS == m_pCI->G.e_MainCard))
      continue;

    m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint();

    m_pCI->G.p_constraint->vConvert_stmColOdn_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vTblColToStmCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MAKEWEAVABLE,_FOOTER_);

  m_pCI->vTransform(__func__, "vTblColToStmCol"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain_reverse()) {

    m_pCI->Analysis.m_pProjection_prior = Card_Projection::pResolvePriorProjection(m_pCI);

    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_tblColOdn_To_stmColOdn(m_pCI);

//    if (m_pCI->G.p_OP->pSub_projection())
//      m_pCI->Analysis.m_pProjection_prior =
//          m_pCI->G.p_OP->pSub_projection();
  }
  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
