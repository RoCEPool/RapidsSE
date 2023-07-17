/*============================================================================*/
/* XF_Cull_Used_bound.cpp -- Cut Sort                                                */
/*============================================================================*/

#include "XF_Cull_Used_bound.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Cull_Used_bound, CN_XF_CULL_USED_BOUND)
__aNewFunction(XF_Cull_Used_bound, CN_XF_CULL_USED_BOUND)
__vDeleteFunction(XF_Cull_Used_bound, CN_XF_CULL_USED_BOUND)
__aFooterFunction(XF_Cull_Used_bound)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Cull_Used_bound::XF_Cull_Used_bound(CardIter* pCI)
: XF_TRANSFORM(CN_XF_CULL_USED_BOUND, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_CULL_USED_BOUND, _FOOTER_);

  _checkInitialised_(CN_XF_CULL_USED_BOUND);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_CULL_USED_BOUND, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Cull_Used_bound::~XF_Cull_Used_bound(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_CULL_USED_BOUND, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Cull_Used_bound::vXF_Cull_Used_bound(CardIter* pCI)
{
  _static_

  XF_Cull_Used_bound* pXF_Cull_Used_bound = new XF_Cull_Used_bound(pCI);

  pXF_Cull_Used_bound->vXF_Cull_Used_bound();

  delete pXF_Cull_Used_bound;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull non-used bounds                                                       */
/*----------------------------------------------------------------------------*/
void XF_Cull_Used_bound::vXF_Cull_Used_bound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_CULL_USED_BOUND, _FOOTER_);

  m_pCI->vTransform(__func__, XF_CULL_USED_BOUND, "cull nused bounds"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->vClearRestart();
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
        if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds)) {
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
        if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds)) {
          //RASSERT(m_pCI->G.p_cmpPred->bIsJoinPred());
          m_pCI->G.p_cmpPred->vMark1(MARK_AT);
        }
      }

      continue;
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->vClearRestart();
//  Card_Constraint::vCullUsedBounds(m_pCI);

  while (m_pCI->G_bEachMain()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      m_pCI->G.p_filter->vCullPred(MARK_AT);
    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint()))
      m_pCI->G.p_constraint->vCullPred(MARK_AT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->vClearRestart();
  Card_Filter::vDeleteEmptyFilters(m_pCI);

//  while (m_pCI->G_bEachMain()) {
//    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
//      m_pCI->G.p_MainCard->vDeleteFilterIfEmpty(m_pCI->G.p_filter);
//  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
