/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_Expr.hpp"
#include "Card_Predicate.hpp"
#include "Card_RlnConj.hpp"
#include "Card_Term.hpp"
#include "Card_SubSet.hpp"
#include "Card_AndConj.hpp"
#include "Card_OrConj.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Filter, CK_o_FILTER)
__aNewFunction(Card_Filter,CN_CARD)
__vDeleteFunction(Card_Filter,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Filter::Card_Filter(void)
: Card_CONDITION(CK_o_FILTER, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Filter::~Card_Filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete empty filters                                                       */
/*----------------------------------------------------------------------------*/
void Card_Filter::vDeleteEmptyFilters(CardIter* pCI) {

  _static_

  while (pCI->bEachOp()) {
    if ((pCI->E.p_filter = pCI->E.p_Op->pSub_filter()))
      ((Card_OP*)pCI->E.p_Op)->vCull_filter_IfEmpty(pCI->E.p_filter);
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vIndividuate_filter(CardIter* pCI)
{
  _static_

  if (!pCI->E.p_Op->pSub_emit())
    pCI->E.p_Op->vDump(__func__);

  RASSERT(pCI->E.p_Op->pSub_emit());

  pCI->E.p_Op->vRemoveChild(pCI->E.p_filter);

  pCI->E.p_filter->vAppendChild(pCI->E.p_Op->pSub_emit()->pClone());

  pCI->E.p_deck->vInsertChildAfter(pCI->E.p_filter, pCI->E.p_Op);

  if (! pCI->E.p_filter->pSub_level()) {
    pCI->E.p_filter->vPushChild(pCI->E.p_Op->pSub_level()->pDupCardRecursive());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vUpdate_expCol(CardIter* pCI, Card_Emit* p_Dic_expr, bool bAlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Predicate* p_predicate = pSub_predicate())
    p_predicate->vUpdate_expCol(pCI, p_Dic_expr,bAlt);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Filter::nCullMarked_rlnConj(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Predicate* p_predicate = pSub_predicate();
  i8 nCulled = p_predicate->nCullMarked_rlnConj(szWithMark);

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vAddTransients(pCI);
}


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Filter::vEarlyProject_recurse_update(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

 // pCI->PRINTBOARD_dbg();

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

 // pCI->PRINTBOARD_dbg();

  ((Card_OP*)pCI->E.p_Op)->vRestate_emit(pCI);

  //pCI->PRINTBOARD_dbg();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Predicate* p_predicate =
      _pCard_Predicate(this->pFindSubCard(CK_PREDICATE));

  if (0 == p_predicate->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_child_mustBe(CK_PREDICATE, CK_NONE);

  if (iDepth > 2) {
   // pCI->PRINTBOARD_dbg();
    vValidationFailed("Filter is at depth %lld"_Stz, iDepth);
  }

  if (iDepth > 1)
    (void)Card::eValidate_parent_either(pCI,
                                       CK_o_CROSSJOIN,
                                       CK_o_FROM,
                                       CK_o_HAVING,
                                       CK_o_INDEXJOIN,
                                       CK_o_INDEXSCAN,
                                       CK_o_SCAN,
                                       CK_o_WHERE,
                                       CK_NONE);

  if (CK_DECK == pParent->eCard()) {
    vValidate_child_mustBe(CK_c_LEVEL, CK_c_EMIT, CK_NONE);
  }

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
