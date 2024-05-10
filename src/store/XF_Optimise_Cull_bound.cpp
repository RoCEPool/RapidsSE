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

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_CULL_BOUND);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();

  while (m_pCI->bEachOp()) {

    if (!(m_pCI->E.p_Op->bIsCard(CK_o_INDEXJOIN) ||
          m_pCI->E.p_Op->bIsCard(CK_o_INDEXSCAN)))
      continue;
    if (!(m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList()))
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

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_CULL_USED_BOUND);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {

    if (! (m_pCI->E.p_Op->bIsCard(CK_o_INDEXJOIN) ||
           m_pCI->E.p_Op->bIsCard(CK_o_INDEXSCAN) ||
           m_pCI->E.p_Op->bIsCard(CK_o_KEYEXISTS) ||
           m_pCI->E.p_Op->bIsCard(CK_o_KEYNOTEXISTS)
    ))
      continue;
    if (! (m_pCI->E.p_bounds = m_pCI->E.p_Op->pSub_bounds()))
      continue;

    if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {

      // have <bounds> & <filter> under <main>

      CardIter* pCI = m_pCI;
      auto bfDerive = [pCI](Card* p_card) {
        Card_RlnConj* p_rlnConj = _pCard_RlnConj(p_card);
        if (p_rlnConj->bSubsetOf_bounds(pCI->E.p_bounds) &&
            ! p_rlnConj->pSymbol()->Y_bHas(P_CrlKeyTest)) {
          //RASSERT(m_pCI->G.p_rlnConj->bIsJoinConj());
          p_rlnConj->vMark1(MARK_AT);
        }

        return C_CONTINUE_RECURSION;
      };

      m_pCI->E.p_filter->bLambdaWalk(CK_RLNCONJ, bfDerive, pCI);

      continue;
    }

    if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {

      // have <bounds> & <filter> under <main>

      CardIter* pCI = m_pCI;
      auto bfDerive = [pCI](Card* p_card) {
        Card_RlnConj* p_rlnConj = _pCard_RlnConj(p_card);
        if (p_rlnConj->bSubsetOf_bounds(pCI->E.p_bounds))
          p_rlnConj->vMark1(MARK_AT);

        return C_CONTINUE_RECURSION;
      };
      m_pCI->E.p_constraint->bLambdaWalk(CK_RLNCONJ, bfDerive, pCI);

      continue;
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();

  i8 nCulled = 0;
  while (m_pCI->bEachOp()) {
    if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter()))
      nCulled += m_pCI->E.p_filter->nCullMarked_rlnConj(MARK_AT);
    if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint()))
      nCulled += m_pCI->E.p_constraint->nCullMarked_rlnConj(MARK_AT);
  }

  if (nCulled)
    bOptRun = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  Card_Filter::vDeleteEmptyFilters(m_pCI);

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
