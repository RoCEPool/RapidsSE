/*============================================================================*/
/* Card_RlnConj.cpp -- Compare Predicate                                      */
/*============================================================================*/

#include "Card_RlnConj.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"
#include "Card_Table.hpp"
#include "Card_Index.hpp"
#include "Card_Bounds.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_RlnConj, CK_RLNCONJ)
__aNewFunction(Card_RlnConj, CN_CARD)
__vDeleteFunction(Card_RlnConj, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RlnConj::Card_RlnConj(void)
: Card_CONJ(CK_RLNCONJ, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_RlnConj::~Card_RlnConj(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clone this compare predicate                                               */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_RlnConj::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_RlnConj(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RlnConj::pGetCorrCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

//  i8 nChildren = this->nChildCards();
//  if (2 != nChildren)
//    return nullptr;

  RASSERT(bIsCorrConj());

  Card* p_child_0 = this->pGetChild(0);
  Card* p_child_1 = this->pGetChild(1);

  if (p_child_0->pSymbol()->bHasFacet(SF_SQY_PRM))
    return _pCard_Term(p_child_1);

  else
  if (p_child_1->pSymbol()->bHasFacet(SF_SQY_PRM))
    return _pCard_Term(p_child_0);

  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_RlnConj::vEnsure_Cast(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Cast xCast_last;
  xCast_last.m_xTypeId = TypeId();
  xCast_last.m_nTypeLength = 0;
  xCast_last.m_nTypeScale = 0;
  RASSERT(! xCast_last.m_xTypeId.bIsValid());

  Kid xCmpConj(this);
  while (xCmpConj.bEach()) {
    Card* pCard = xCmpConj.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ:
        _pCard_AndConj(pCard)->vEnsure_Cast(pCI); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vEnsure_Cast(pCI); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vEnsure_Cast(pCI);  break;
      case CK_RLNCONJ:
        _pCard_RlnConj(pCard)->vEnsure_Cast(pCI); break;


      case CK_TERM:
        _pCard_Term(pCard)->vEnsure_Cast(pCI, xCast_last);  break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get the first table                                                        */
/*----------------------------------------------------------------------------*/
char Card_RlnConj::cGetTable_a(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_a = _pCard_Term(pGetChild(0));
  char cTableSyn_this_a = p_term_this_a->pSymbol()->cTblSyn();

  return cTableSyn_this_a;
}

/*----------------------------------------------------------------------------*/
/* get the second table                                                       */
/*----------------------------------------------------------------------------*/
char Card_RlnConj::cGetTable_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_b = _pCard_Term(pGetChild(1));
  char cTableSyn_this_b = p_term_this_b->pSymbol()->cTblSyn();

  return cTableSyn_this_b;
}

/*----------------------------------------------------------------------------*/
/* get the first term                                                         */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RlnConj::pGetTerm_0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_0 = _pCard_Term( pGetChild(0) );

  return p_term_0;
}

/*----------------------------------------------------------------------------*/
/* get the second term                                                        */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RlnConj::pGetTerm_1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_1 = _pCard_Term( pGetChild(1) );

  return p_term_1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bIsEq(Card_RlnConj* p_rlnConj_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (nChildCards() != p_rlnConj_arg->nChildCards())
    return false;

  for (Kid xKid(this); xKid.bEach(); /**/) {
    Card_Term* p_term_this = xKid.p_term();
    Card_Term* p_term_that = p_rlnConj_arg->pGetTerm(xKid.i_Child());

    Symbol* pSymbol_this = p_term_this->pSymbol();
    Symbol* pSymbol_that = p_term_that->pSymbol();

    if (!pSymbol_this->bMatches(pSymbol_that))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bIsEqOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bHasFacet("=="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bIsRlnOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bHasFacet("<"_Stz))  return true;
  if (pSymbol()->bHasFacet("<="_Stz)) return true;
  if (pSymbol()->bHasFacet("=="_Stz)) return true;
  if (pSymbol()->bHasFacet("!="_Stz)) return true;
  if (pSymbol()->bHasFacet(">="_Stz)) return true;
  if (pSymbol()->bHasFacet(">"_Stz))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a bounds for given index                                 */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bIsIndexBound(Card_Index* p_index)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_index)
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* pTerm_0 = _pCard_Term(this->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(this->pGetChild(1));

  if (pTerm_0->pSymbol()->bHasFacet(SF_TBL_COL)) {
    // + term_0 is $c.n

    if (pTerm_1->pSymbol()->bHasFacet(SF_TBL_COL)) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (p_index->bTblCol_IsKeyIn_index(pTerm_0->szFacet(SF_TBL_COL)) ||
          p_index->bTblCol_IsKeyIn_index(pTerm_1->szFacet(SF_TBL_COL)) )
        return true;

      return false;
    }
    else {
      // + only term_0 is $c.n

      // ? is term_0:$c.n an index column
      if (p_index->bTblCol_IsKeyIn_index(pTerm_0->szFacet(SF_TBL_COL)))
        return true;

      return false;
    }
  }
  else
  if (pTerm_1->pSymbol()->bHasFacet(SF_TBL_COL)) {
    // + only term_1 is $c.n

    // ? is term_0:$c.n an index column
    if (p_index->bTblCol_IsKeyIn_index(pTerm_1->szFacet(SF_TBL_COL)) )
      return true;

    return false;
  }

  // + neither term_0 nor term_1 is $c.n
  return false;
}
/*----------------------------------------------------------------------------*/
/* get the other table referenced by this join/cmpPred given a first table    */
/*----------------------------------------------------------------------------*/
char Card_RlnConj::cOtherTable(char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->pSymbol()->cTblSyn();
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->pSymbol()->cTblSyn();

  if (cTableSyn_this_0 == cTblSyn)
    return cTableSyn_this_1;
  else
    return cTableSyn_this_0;
}

/*----------------------------------------------------------------------------*/
/* is given cmpPred a subset of bounds?                                       */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bSubsetOf_bounds(Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xBounds(p_bounds);
  while (xBounds.bEach(CK_BOUND)) {
    Card_Bound* p_bound = xBounds.p_bound();
    Symbol* pSym_bound = p_bound->pSymbol();

    Card* p_child1_bound = p_bound->pGetChild(1);
    Stz* sz_bnd_expCol = p_child1_bound->szFacet(SF_NONE);

    Symbol* pSym_rlnConj = pSymbol();
    Stz* sz_cmp_arg_0  = this->pGetChild(0)->szFacet(SF_NONE);
    Stz* sz_cmp_arg_1  = this->pGetChild(1)->szFacet(SF_NONE);

    if (! pSym_bound->bMatches(pSym_rlnConj))
      continue;

    // todo: need bounds to be bndTyp , etc
    // anyways...

    if (sz_cmp_arg_0->bSame(sz_bnd_expCol))
      return true;
    if (sz_cmp_arg_1->bSame(sz_bnd_expCol))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* does this cmpPred reference given table?                                   */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bUsesTable(char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->pSymbol()->cTblSyn();
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->pSymbol()->cTblSyn();

  if (cTableSyn_this_0 == cTblSyn || cTableSyn_this_1 == cTblSyn)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_RlnConj::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  RASSERT( m_xSymbol.bHasFacet(SF_BOL_FN) ||
           m_xSymbol.bHasFacet(SF_RLN_OP) ||
           m_xSymbol.bHasFacet(SF_SQY_OP) );

  Kid xRpnCalc(this);
  while (xRpnCalc.bEach()) {
    Card* pCard = xRpnCalc.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vValidate(pCI, iDepth + 1, this);  break;
      case CK_RLNCONJ:
        _pCard_RlnConj(pCard)->vValidate(pCI, iDepth + 1, this); break;

      case CK_TERM:    _pCard_Term(pCard)->vValidate(pCI,iDepth+1,this);    break;

      default:
        vValidationFailed("<<replace>> %p"_Stz,this);
    }
  }

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
