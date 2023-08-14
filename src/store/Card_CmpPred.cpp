/*============================================================================*/
/* Card_CmpPred.cpp -- Compare Predicate                                      */
/*============================================================================*/

#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"
#include "Card_Table.hpp"
#include "Card_Index.hpp"
#include "Card_Bounds.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CmpPred, CK_CMPPRED)
__aNewFunction(Card_CmpPred,CN_CARD)
__vDeleteFunction(Card_CmpPred,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CmpPred::Card_CmpPred(void)
: Card_PRED(CK_CMPPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred::~Card_CmpPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate cast kind of this cmpPred given the l & r table partitions       */
/*----------------------------------------------------------------------------*/
eXK Card_CmpPred::eCastKind(char cTblSyn_l, Card_TablePtn* p_tablePtn_l,
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
Card_CmpPred* Card_CmpPred::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_CmpPred(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index               */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_CmpPred::pCmpPredFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* pTerm2 = _pCard_Term(this->pGet(2));
  if (eBound != pTerm2->eEvalBound())
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(this->pGet(0));
  Stz* szTerm0 = pTerm0->szSymbol(SF_NONE);
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;

  return _pCard_CmpPred(this);
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bCmpPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Card_Term* pTerm1 = _pCard_Term(pCard_PredCmp->pGet(1));

  Card_Term* pTerm2 = _pCard_Term(this->pGet(2));
  if (PB_NONE == pTerm2->eEvalBound())
    return false;

  Card_Term* pTerm0 = _pCard_Term(this->pGet(0));
  Stz* szTerm0 = pTerm0->szSymbol(SF_NONE);
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bCoPtnWith(Card_CmpPred* p_cmpPred_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szSymbol(SF_NONE)->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpPred_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpPred_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szSymbol(SF_NONE)->azChar()[1];

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
Card_Term* Card_CmpPred::pGetCorrCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

//  i8 nChildren = this->nChildCards();
//  if (2 != nChildren)
//    return nullptr;

  RASSERT(bIsCorrPred());

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
void Card_CmpPred::vEnsureTypesFor_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xCmpPred(this);
  while (xCmpPred.bEach()) {
    Card* pCard = xCmpPred.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDPRED: _pCard_AndPred(pCard)->vEnsureTypesFor_stmOdn(pCI); break;
      case CK_CMPPRED: _pCard_CmpPred(pCard)->vEnsureTypesFor_stmOdn(pCI); break;
      case CK_NOTPRED: _pCard_NotPred(pCard)->vEnsureTypesFor_stmOdn(pCI); break;
      case CK_ORPRED:  _pCard_OrPred(pCard)->vEnsureTypesFor_stmOdn(pCI);  break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeFor_stmOdn(pCI);     break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CmpPred::pExtractParamTerm(void)
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
char Card_CmpPred::cGetTable_a(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_a = _pCard_Term(pGetChild(0));
  char cTableSyn_this_a = p_term_this_a->szSymbol(SF_NONE)->azChar()[1];

  return cTableSyn_this_a;
}

/*----------------------------------------------------------------------------*/
/* get the second table                                                       */
/*----------------------------------------------------------------------------*/
char Card_CmpPred::cGetTable_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_b = _pCard_Term(pGetChild(1));
  char cTableSyn_this_b = p_term_this_b->szSymbol(SF_NONE)->azChar()[1];

  return cTableSyn_this_b;
}

/*----------------------------------------------------------------------------*/
/* get both tables                                                            */
/*----------------------------------------------------------------------------*/
void Card_CmpPred::vGetTables_a_b(char& rcTblSyn_a, char& rcTblSyn_b)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  rcTblSyn_a = cGetTable_a();
  rcTblSyn_b = cGetTable_b();
}

/*----------------------------------------------------------------------------*/
/* get the first term                                                         */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CmpPred::pGetTerm_0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_0 = _pCard_Term( pGetChild(0) );

  return p_term_0;
}

/*----------------------------------------------------------------------------*/
/* get the second term                                                        */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CmpPred::pGetTerm_1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_1 = _pCard_Term( pGetChild(1) );

  return p_term_1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bIsEq(Card_CmpPred* p_cmpPred_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szSymbol(SF_NONE)->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpPred_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpPred_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szSymbol(SF_NONE)->azChar()[1];

  if (cTableSyn_this_0 == cTableSyn_arg_0 &&
      cTableSyn_this_1 == cTableSyn_arg_1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bIsEqOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bSame("=="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get the other table referenced by this join/cmpPred given a first table    */
/*----------------------------------------------------------------------------*/
char Card_CmpPred::cOtherTable(char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szSymbol(SF_NONE)->azChar()[1];

  if (cTableSyn_this_0 == cTblSyn)
    return cTableSyn_this_1;
  else
    return cTableSyn_this_0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bOverlapsWith(Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szSymbol(SF_NONE)->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szSymbol(SF_NONE)->azChar()[1];

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
bool Card_CmpPred::bSubsetOf_bounds(Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(p_bounds);
  while (xCI.G_bEachMain(CK_BOUND)) {

//    Stz* sz_bnd_idxColOdn = xCI.G.p_bound->pGetChild(0)->szSymbol(SF_NONE);
    Stz* sz_bnd_stmColOdn = xCI.G.p_bound->pGetChild(1)->szSymbol(SF_NONE);
    Stz* sz_bnd_rlnOpr    = xCI.G.p_bound->szSymbol(SF_NONE);

    Stz* sz_cmp_rlnOpr    = this->szSymbol(SF_NONE);
    Stz* sz_cmp_arg_0     = this->pGetChild(0)->szSymbol(SF_NONE);
    Stz* sz_cmp_arg_1     = this->pGetChild(1)->szSymbol(SF_NONE);

    if (! sz_cmp_rlnOpr->bSame(sz_bnd_rlnOpr))
      continue;

    // todo: need bounds to be bndTyp , etc
    // anyways...

    if (sz_cmp_arg_0->bSame(sz_bnd_stmColOdn))
      return true;
    if (sz_cmp_arg_1->bSame(sz_bnd_stmColOdn))
      return true;

  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* does this cmpPred reference given table?                                   */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bUsesTable(char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szSymbol(SF_NONE)->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szSymbol(SF_NONE)->azChar()[1];

  if (cTableSyn_this_0 == cTblSyn || cTableSyn_this_1 == cTblSyn)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this cmpPred reference given both tables?                             */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bUsesTables(char cTblSyn_a, char cTblSyn_b)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bUsesTable(cTblSyn_a) && bUsesTable(cTblSyn_b))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_CmpPred::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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

      case CK_ANDPRED: _pCard_AndPred(pCard)->vValidate(pCI,iDepth+1,this); break;
      case CK_CMPPRED: _pCard_CmpPred(pCard)->vValidate(pCI,iDepth+1,this); break;
      case CK_NOTPRED: _pCard_NotPred(pCard)->vValidate(pCI,iDepth+1,this); break;
      case CK_ORPRED:  _pCard_OrPred(pCard)->vValidate(pCI,iDepth+1,this);  break;

      case CK_TERM:    _pCard_Term(pCard)->vValidate(pCI,iDepth+1,this);    break;

      default:
        NEVER_REACH
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
