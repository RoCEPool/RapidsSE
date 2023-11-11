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
/* find a comparePred which is the E_eTerm_Bound of given index               */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_RlnConj::pCmpPredFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* pTerm2 = _pCard_Term(this->pGet(2));
  if (eBound != pTerm2->eEvalBound())
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(this->pGet(0));
  Stz* szTerm0 = pTerm0->szFacet(SF_NONE);
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;

  return _pCard_RlnConj(this);
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

  if (p_child_0->pSymbol()->bIs_Sqy_Prm())
    return _pCard_Term(p_child_1);

  else
  if (p_child_1->pSymbol()->bIs_Sqy_Prm())
    return _pCard_Term(p_child_0);

  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_RlnConj::vEnsureTypeId_expCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xCmpConj(this);
  while (xCmpConj.bEach()) {
    Card* pCard = xCmpConj.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ:
        _pCard_AndConj(pCard)->vEnsureTypeId_expCol(pCI); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vEnsureTypeId_expCol(pCI); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vEnsureTypeId_expCol(pCI);  break;
      case CK_RLNCONJ:
        _pCard_RlnConj(pCard)->vEnsureTypeId_expCol(pCI); break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeId_expCol(pCI);     break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RlnConj::pExtractParamTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = this->nChildCards();
  RASSERT(2 == nChildren);

  Card* p_child_0 = this->pGetChild(0);
  Card* p_child_1 = this->pGetChild(1);

  if (p_child_0->pSymbol()->bIs_Sqy_Prm())
    return _pCard_Term(p_child_0);

  else
  if (p_child_1->pSymbol()->bIs_Sqy_Prm())
    return _pCard_Term(p_child_1);

  else
    NEVER_REACH;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the first table                                                        */
/*----------------------------------------------------------------------------*/
char Card_RlnConj::cGetTable_a(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_a = _pCard_Term(pGetChild(0));
  char cTableSyn_this_a = p_term_this_a->szFacet(SF_NONE)->azChar()[1];

  return cTableSyn_this_a;
}

/*----------------------------------------------------------------------------*/
/* get the second table                                                       */
/*----------------------------------------------------------------------------*/
char Card_RlnConj::cGetTable_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_b = _pCard_Term(pGetChild(1));
  char cTableSyn_this_b = p_term_this_b->szFacet(SF_NONE)->azChar()[1];

  return cTableSyn_this_b;
}

/*----------------------------------------------------------------------------*/
/* get both tables                                                            */
/*----------------------------------------------------------------------------*/
void Card_RlnConj::vGetTables_a_b(char& rcTblSyn_a, char& rcTblSyn_b)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  rcTblSyn_a = cGetTable_a();
  rcTblSyn_b = cGetTable_b();
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

    if (! pSymbol_this->bMatches(pSymbol_that))
      return false;
  }

  return true;
}
/*----------------------------------------------------------------------------*/
//bool Card_RlnConj::bIsEq(Card_RlnConj* p_rlnConj_arg)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
//  char cTableSyn_this_0 = p_term_this_0->szFacet(SF_NONE)->azChar()[1];
//  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
//  char cTableSyn_this_1 = p_term_this_1->szFacet(SF_NONE)->azChar()[1];
//
//  Card_Term* p_term_arg_0 = _pCard_Term(p_rlnConj_arg->pGetChild(0));
//  char cTableSyn_arg_0 = p_term_arg_0->szFacet(SF_NONE)->azChar()[1];
//  Card_Term* p_term_arg_1 = _pCard_Term(p_rlnConj_arg->pGetChild(1));
//  char cTableSyn_arg_1 = p_term_arg_1->szFacet(SF_NONE)->azChar()[1];
//
//  if (cTableSyn_this_0 == cTableSyn_arg_0 &&
//      cTableSyn_this_1 == cTableSyn_arg_1)
//    return true;
//
//  return false;
//}

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

  if (pTerm_0->pSymbol()->bIs_Tbl_Col()) {
    // + term_0 is $c.n

    if (pTerm_1->pSymbol()->bIs_Tbl_Col()) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (p_index->bTblCol_IsKeyIn_index(pTerm_0->szFacet(SF_NONE)) ||
          p_index->bTblCol_IsKeyIn_index(pTerm_1->szFacet(SF_NONE)) )
        return true;

      return false;
    }
    else {
      // + only term_0 is $c.n

      // ? is term_0:$c.n an index column
      if (p_index->bTblCol_IsKeyIn_index(pTerm_0->szFacet(SF_NONE)))
        return true;

      return false;
    }
  }
  else
  if (pTerm_1->pSymbol()->bIs_Tbl_Col()) {
    // + only term_1 is $c.n

    // ? is term_0:$c.n an index column
    if (p_index->bTblCol_IsKeyIn_index(pTerm_1->szFacet(SF_NONE)) )
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
  char cTableSyn_this_0 = p_term_this_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szFacet(SF_NONE)->azChar()[1];

  if (cTableSyn_this_0 == cTblSyn)
    return cTableSyn_this_1;
  else
    return cTableSyn_this_0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bOverlapsWith(Card_RlnConj* p_conjCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szFacet(SF_NONE)->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_conjCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_conjCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szFacet(SF_NONE)->azChar()[1];

  if (   cTableSyn_this_0 == cTableSyn_arg_0
      || cTableSyn_this_0 == cTableSyn_arg_1
      || cTableSyn_this_1 == cTableSyn_arg_0
      || cTableSyn_this_1 == cTableSyn_arg_1
      )
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is given cmpPred a subset of bounds?                                       */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bSubsetOf_bounds(Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(p_bounds);
  while (xKid.bEach(CK_BOUND)) {

//    Stz* sz_bnd_idxCol = xKid.G.p_bound->pGetChild(0)->szFacet(SF_NONE);
    Stz* sz_bnd_stmCol = xKid.p_bound()->pGetChild(1)->szFacet(SF_NONE);
    Stz* sz_bnd_rlnOpr    = xKid.p_bound()->szFacet(SF_NONE);

    Stz* sz_cmp_rlnOpr    = this->szFacet(SF_NONE);
    Stz* sz_cmp_arg_0     = this->pGetChild(0)->szFacet(SF_NONE);
    Stz* sz_cmp_arg_1     = this->pGetChild(1)->szFacet(SF_NONE);

    if (! sz_cmp_rlnOpr->bSame(sz_bnd_rlnOpr))
      continue;

    // todo: need bounds to be bndTyp , etc
    // anyways...

    if (sz_cmp_arg_0->bSame(sz_bnd_stmCol))
      return true;
    if (sz_cmp_arg_1->bSame(sz_bnd_stmCol))
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
  char cTableSyn_this_0 = p_term_this_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szFacet(SF_NONE)->azChar()[1];

  if (cTableSyn_this_0 == cTblSyn || cTableSyn_this_1 == cTblSyn)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this cmpPred reference given both tables?                             */
/*----------------------------------------------------------------------------*/
bool Card_RlnConj::bUsesTables(char cTblSyn_a, char cTblSyn_b)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bUsesTable(cTblSyn_a) && bUsesTable(cTblSyn_b))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_RlnConj::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

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
