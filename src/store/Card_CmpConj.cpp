/*============================================================================*/
/* Card_CmpConj.cpp -- Compare Predicate                                      */
/*============================================================================*/

#include "Card_CmpConj.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"
#include "Card_Table.hpp"
#include "Card_Index.hpp"
#include "Card_Bounds.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CmpConj, CK_CMPCONJ)
__aNewFunction(Card_CmpConj, CN_CARD)
__vDeleteFunction(Card_CmpConj, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CmpConj::Card_CmpConj(void)
: Card_CONJUNCTION(CK_CMPCONJ, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CmpConj::~Card_CmpConj(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate cast kind of this cmpPred given the l & r table partitions       */
/*----------------------------------------------------------------------------*/
eXK Card_CmpConj::eCastKind(char cTblSyn_l, Card_TablePtn* p_tablePtn_l,
                            char cTblSyn_r, Card_TablePtn* p_tablePtn_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(cTblSyn_l);
  UNUSED(cTblSyn_r);

  // todo: alka and ray to discuss

  Card_Term* p_term_0 = pGetTerm_0();
  Card_Term* p_term_1 = pGetTerm_1();

  if ((p_tablePtn_r->bSameSymbol(p_term_0) && p_tablePtn_l->bSameSymbol(p_term_1)) ||
      (p_tablePtn_r->bSameSymbol(p_term_1) && p_tablePtn_l->bSameSymbol(p_term_0)))
      return XK_UNICAST;
  else
  if (p_tablePtn_r->bSameSymbol(p_term_0) || p_tablePtn_r->bSameSymbol(p_term_1))
    return XK_HASHCAST;
  else
    return XK_BROADCAST;
}

/*----------------------------------------------------------------------------*/
/* clone this compare predicate                                               */
/*----------------------------------------------------------------------------*/
Card_CmpConj* Card_CmpConj::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_CmpConj(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index               */
/*----------------------------------------------------------------------------*/
Card_CmpConj* Card_CmpConj::pCmpPredFindBound(Index* pIndex, ePB eBound)
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

  return _pCard_CmpConj(this);
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_CmpConj::bCmpPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Card_Term* pTerm1 = _pCard_Term(pCard_PredCmp->pGet(1));

  Card_Term* pTerm2 = _pCard_Term(this->pGet(2));
  if (PB_NONE == pTerm2->eEvalBound())
    return false;

  Card_Term* pTerm0 = _pCard_Term(this->pGet(0));
  Stz* szTerm0 = pTerm0->szFacet(SF_NONE);
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Card_CmpConj::bCoPtnWith(Card_CmpConj* p_cmpConj_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szFacet(SF_NONE)->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpConj_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpConj_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szFacet(SF_NONE)->azChar()[1];

  bool bAreCoPtn = false;

  if (   cTableSyn_arg_0 == cTableSyn_this_0
         || cTableSyn_arg_0 == cTableSyn_this_1
         || cTableSyn_arg_1 == cTableSyn_this_0
         || cTableSyn_arg_1 == cTableSyn_this_1
      )
    bAreCoPtn = true;

  vTrace("(%c,%c) - (%c,%c) are%s co-partitioned\n"_Stz,
         cTableSyn_this_0,cTableSyn_this_1,
         cTableSyn_arg_0,cTableSyn_arg_1,
         bAreCoPtn ? "" : " NOT");

  return bAreCoPtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CmpConj::pGetCorrCol(void)
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
void Card_CmpConj::vEnsureTypesFor_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xCmpConj(this);
  while (xCmpConj.bEach()) {
    Card* pCard = xCmpConj.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_CMPCONJ: _pCard_CmpConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_NOTCONJ: _pCard_NotConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_ORCONJ:  _pCard_OrConj(pCard)->vEnsureTypesFor_stmCol(pCI);  break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeFor_stmCol(pCI);     break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CmpConj::pExtractParamTerm(void)
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
char Card_CmpConj::cGetTable_a(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_a = _pCard_Term(pGetChild(0));
  char cTableSyn_this_a = p_term_this_a->szFacet(SF_NONE)->azChar()[1];

  return cTableSyn_this_a;
}

/*----------------------------------------------------------------------------*/
/* get the second table                                                       */
/*----------------------------------------------------------------------------*/
char Card_CmpConj::cGetTable_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_b = _pCard_Term(pGetChild(1));
  char cTableSyn_this_b = p_term_this_b->szFacet(SF_NONE)->azChar()[1];

  return cTableSyn_this_b;
}

/*----------------------------------------------------------------------------*/
/* get both tables                                                            */
/*----------------------------------------------------------------------------*/
void Card_CmpConj::vGetTables_a_b(char& rcTblSyn_a, char& rcTblSyn_b)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  rcTblSyn_a = cGetTable_a();
  rcTblSyn_b = cGetTable_b();
}

/*----------------------------------------------------------------------------*/
/* get the first term                                                         */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CmpConj::pGetTerm_0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_0 = _pCard_Term( pGetChild(0) );

  return p_term_0;
}

/*----------------------------------------------------------------------------*/
/* get the second term                                                        */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CmpConj::pGetTerm_1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_1 = _pCard_Term( pGetChild(1) );

  return p_term_1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpConj::bIsEq(Card_CmpConj* p_cmpConj_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szFacet(SF_NONE)->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpConj_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szFacet(SF_NONE)->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpConj_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szFacet(SF_NONE)->azChar()[1];

  if (cTableSyn_this_0 == cTableSyn_arg_0 &&
      cTableSyn_this_1 == cTableSyn_arg_1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpConj::bIsEqOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bSame("=="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get the other table referenced by this join/cmpPred given a first table    */
/*----------------------------------------------------------------------------*/
char Card_CmpConj::cOtherTable(char cTblSyn)
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
bool Card_CmpConj::bOverlapsWith(Card_CmpConj* p_conjCmp_arg)
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
bool Card_CmpConj::bSubsetOf_bounds(Card_Bounds* p_bounds)
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
bool Card_CmpConj::bUsesTable(char cTblSyn)
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
bool Card_CmpConj::bUsesTables(char cTblSyn_a, char cTblSyn_b)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bUsesTable(cTblSyn_a) && bUsesTable(cTblSyn_b))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_CmpConj::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
      case CK_CMPCONJ:
        _pCard_CmpConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vValidate(pCI, iDepth + 1, this);  break;

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
