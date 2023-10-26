/*============================================================================*/
/* XF_Optimise_Cull_bound.cpp -- Cut Sort                                                */
/*============================================================================*/

#include "XF_Optimise_Cull_bound.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Cull_bound, CN_XF_OPTIMISE_CULL_BOUND)
__aNewFunction(XF_Optimise_Cull_bound, CN_XF_OPTIMISE_CULL_BOUND)
__vDeleteFunction(XF_Optimise_Cull_bound, CN_XF_OPTIMISE_CULL_BOUND)
__aFooterFunction(XF_Optimise_Cull_bound)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Cull_bound::XF_Optimise_Cull_bound(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_CULL_BOUND, OT_CULL_BOUND, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CULL_BOUND, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_CULL_BOUND);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CULL_BOUND, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Cull_bound::~XF_Optimise_Cull_bound(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CULL_BOUND, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Cull_bound::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Cull_bound* pXF_Cull_NonUsed_bound = new XF_Optimise_Cull_bound(pCI);

  bool b1 = pXF_Cull_NonUsed_bound->bXF_Cull_NonUsed_bound();
  bool b2 = pXF_Cull_NonUsed_bound->bXF_Cull_Used_bound();

  pXF_Cull_NonUsed_bound->vOptimised(b1 | b2);

  delete pXF_Cull_NonUsed_bound;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull non-used bounds                                                       */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Cull_bound::bXF_Cull_NonUsed_bound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CULL_BOUND, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_CULL_NONUSED_BOUND, "cull non-used bounds"_Stz);

  m_pCI->vClearRestart();

  while (m_pCI->G_bEachMain_NONE()) {

    if (!(m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
          m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (!(m_pCI->G.p_indexList = m_pCI->G.p_MainCard->pSub_indexList()))
      continue;

    // have <bounds> & <filter> under <main>
    if (Card_Index::nCullNonusedBounds(m_pCI))
      bOptRun = true;

  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/* cull non-used bounds                                                       */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Cull_bound::bXF_Cull_Used_bound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_CULL_BOUND, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_CULL_USED_BOUND, "cull nused bounds"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {

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

      m_pCI->vRecurseBegin(m_pCI->G.p_filter);
      while ((m_pCI->G.p_rlnConj = _pCard_RlnConj(m_pCI->pRecurseEach(CK_RLNCONJ)))) {
        if (m_pCI->G.p_rlnConj->bSubsetOf_bounds(m_pCI->G.p_bounds)) {
          //RASSERT(m_pCI->G.p_rlnConj->bIsJoinConj());
          m_pCI->G.p_rlnConj->vMark1(MARK_AT);
        }
      }
      m_pCI->vRecurseEnd();
      continue;
    }

    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {

      // have <bounds> & <filter> under <main>

       m_pCI->vRecurseBegin(m_pCI->G.p_constraint);
      while ((m_pCI->G.p_rlnConj = _pCard_RlnConj(m_pCI->pRecurseEach(CK_RLNCONJ)))) {
        if (m_pCI->G.p_rlnConj->bSubsetOf_bounds(m_pCI->G.p_bounds)) {
          //RASSERT(m_pCI->G.p_rlnConj->bIsJoinConj());
          m_pCI->G.p_rlnConj->vMark1(MARK_AT);
        }
      }

      m_pCI->vRecurseEnd();
      continue;
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->vClearRestart();

  i8 nCulled = 0;
  while (m_pCI->G_bEachMain_NONE()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      nCulled += m_pCI->G.p_filter->nCullConj(MARK_AT);
    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint()))
      nCulled += m_pCI->G.p_constraint->nCullConj(MARK_AT);
  }

  if (nCulled)
    bOptRun = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->vClearRestart();
  Card_Filter::vDeleteEmptyFilters(m_pCI);

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
