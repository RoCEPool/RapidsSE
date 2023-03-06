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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CmpPred::Card_CmpPred(Stz* szCmp)
    : Card_PRED(CK_CMPPRED, szCmp)
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

  // todo: alka and ray to discuss

  Card_Term* p_term_0 = pGetTerm_0();
  Card_Term* p_term_1 = pGetTerm_1();

  if ( (p_tablePtn_r->bSameValue(p_term_0) && p_tablePtn_l->bSameValue(p_term_1)) ||
       (p_tablePtn_r->bSameValue(p_term_1) && p_tablePtn_l->bSameValue(p_term_0)))
      return XK_UNICAST;
  else
  if (p_tablePtn_r->bSameValue(p_term_0) || p_tablePtn_r->bSameValue(p_term_1))
    return XK_HASHCAST;
  else
    return XK_BROADCAST;
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
  Stz* szTerm0 = pTerm0->sz();
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
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bContains_Sqy_Call(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (Card* p_card = xCI.pEachChild()) {
    if (p_card->bIsCard(CK_TERM) &&
        pCI->m_pBench->bIsSubQueryNm(p_card->szValue()))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bCoPtnWith(Card_CmpPred* p_cmpPred_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpPred_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szValue()->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpPred_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szValue()->azChar()[1];

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
Card_Term* Card_CmpPred::pExtractArgTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = this->nChildCards();
  if (2 != nChildren)
    return nullptr;

  Card* p_child_0 = this->pGetChild(0);
  Card* p_child_1 = this->pGetChild(1);

  if (Card::bIsParamOrdinal(p_child_0->szValue()))
    return _pCard_Term(p_child_1);

  else
  if (Card::bIsParamOrdinal(p_child_1->szValue()))
    return _pCard_Term(p_child_0);

  else
    return nullptr;
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

  if (Card::bIsParamOrdinal(p_child_0->szValue()))
    return _pCard_Term(p_child_0);

  else
  if (Card::bIsParamOrdinal(p_child_1->szValue()))
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
  char cTableSyn_this_a = p_term_this_a->szValue()->azChar()[1];

  return cTableSyn_this_a;
}

/*----------------------------------------------------------------------------*/
/* get the second table                                                       */
/*----------------------------------------------------------------------------*/
char Card_CmpPred::cGetTable_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_b = _pCard_Term(pGetChild(1));
  char cTableSyn_this_b = p_term_this_b->szValue()->azChar()[1];

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
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_cmpPred_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szValue()->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_cmpPred_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szValue()->azChar()[1];

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

  if (bSame("=="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this cmpPred a joinPred between the two given tables?                   */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bIsJoinPredBetween(char cTblSyn_l, char cTblSyn_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // todo: alka and ray to discuss

  return false;
}

/*----------------------------------------------------------------------------*/
/* mark the bounds of this <predCmp>                                          */
/*----------------------------------------------------------------------------*/
void Card_CmpPred::vMarkBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szCmp = this->sz();

  char cBound = '?';

  if (szCmp->bSame("<"_Stz))  cBound = 'U';
  else if (szCmp->bSame("<="_Stz)) cBound = 'U';
  else if (szCmp->bSame("="_Stz))  cBound = 'E';
  else if (szCmp->bSame("=="_Stz)) cBound = 'E';
  else if (szCmp->bSame("!="_Stz)) cBound = 'N';
  else if (szCmp->bSame(">="_Stz)) cBound = 'L';
  else if (szCmp->bSame(">"_Stz))  cBound = 'L';

  this->vMark1(cBound);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bMaybeBounds_v2(Card_Table* p_table, Card_Index* p_index,
                                   bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_index)
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* pTerm_0 = _pCard_Term(this->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(this->pGetChild(1));

  if (Card::bIsTableColOrdinal(pTerm_0->szValue())) {
    // + term_0 is $c.n

    if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szValue()) ||
          p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szValue()) )
        return true;

      return false;
    }
    else {
      // + term_0 is $c.n

      // ? check term_1 is permitted param
      if ((! bParamAllowedInBounds) &&
          Card::bIsParamOrdinal(pTerm_1->szValue()))
        return false;

      // ? is term_0:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szValue()))
        return true;

      return false;
    }
  }
  else
  if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
    // + term_1 is $c.n

    // ? maybe term_0 is permitted param
    if ((! bParamAllowedInBounds) &&
        Card::bIsParamOrdinal(pTerm_0->szValue()))
      return false;

    // ? is term_0:$c.n an index column
    if (p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szValue()) )
      return true;

    return false;
  }
  else {
    // + neither term is $c.n
    return false;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bMaybeBoundsOf_index(Card_Index* p_index, bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* pTerm_0 = _pCard_Term(this->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(this->pGetChild(1));

  if (Card::bIsTableColOrdinal(pTerm_0->szValue())) {
    // + term_0 is $c.n

    if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szValue()) ||
          p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szValue()) )
        return true;

      // - neither term are a $c.n matching index column
      return false;
    }
    else {
      // + term_0 is $c.n

      // ? check term_1 is permitted param
      if ((! bParamAllowedInBounds) &&
          Card::bIsParamOrdinal(pTerm_1->szValue()))
        return false;

      // ? is term_0:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szValue()))
        return true;

      // - term_0 is a $c.n but doesn't match index column
      return false;
    }
  }
  else
  if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
    // + term_1 is $c.n

    // ? maybe term_0 is permitted param
    if ((! bParamAllowedInBounds) &&
        Card::bIsParamOrdinal(pTerm_0->szValue()))
      return false;

    // ? is term_0:$c.n an index column
    if (p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szValue()) )
      return true;

    // - term_1 is a $c.n but doesn't match index column
    return false;
  }
  else {
    // + neither term is $c.n
    return false;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bMaybeJoinPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! bIsEqOp())
      return false;

  Card_Term* p_term_0 = _pCard_Term(pGetChild(0));
  Card_Term* p_term_1 = _pCard_Term(pGetChild(1));
  if ( p_term_0->cValue() == '$' && p_term_1->cValue() == '$')
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
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

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
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

  Card_Term* p_term_arg_0 = _pCard_Term(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_term_arg_0->szValue()->azChar()[1];
  Card_Term* p_term_arg_1 = _pCard_Term(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_term_arg_1->szValue()->azChar()[1];

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
  while (Card* p_bound = xCI.pEachChild()) {

    Card_Term* p_term_0 = _pCard_Term(p_bound->pGetChild(0));
    //Stz* szBound = p_term_0->szValue();

    Stz* sz_bnd_idxColOdn = p_bound->pGetChild(1)->szValue();
    Stz* sz_bnd_stmColOdn = p_bound->pGetChild(2)->szValue();
    Stz* sz_bnd_rlnOpr    = p_bound->pGetChild(3)->szValue();

    Stz* sz_cmp_rlnOpr    = this->szValue();
    Stz* sz_cmp_arg_0     = this->pGetChild(0)->szValue();
    Stz* sz_cmp_arg_1     = this->pGetChild(1)->szValue();

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
/* does this cmpPred reference given table?                                   */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bUsesTable(char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_this_0 = _pCard_Term(pGetChild(0));
  char cTableSyn_this_0 = p_term_this_0->szValue()->azChar()[1];
  Card_Term* p_term_this_1 = _pCard_Term(pGetChild(1));
  char cTableSyn_this_1 = p_term_this_1->szValue()->azChar()[1];

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
