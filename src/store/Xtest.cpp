/*============================================================================*/
/* Xtest.cpp --                                                       */
/*============================================================================*/

#include "Xtest.hpp"

#include "Index.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xtest)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xtest::Xtest(eCN eClass)
: Xextract(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xtest::~Xtest(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0];
  char cSecond = szColumnRef->azChar()[1];
  char cThird  = szColumnRef->azChar()[2];
  char cFourth = szColumnRef->azChar()[3];

  if ('$' == cFirst && '$' == cSecond && '.' == cFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_orPred->pGet(0);
  if (T_bPRED_HasBounds(pSubPred0, pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)p_orPred->pGet(1);
  if (T_bPRED_HasBounds(pSubPred1, pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_andPred->pGet(0);
  if (T_bPRED_HasBounds(pSubPred0, pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)p_andPred->pGet(1);
  if (T_bPRED_HasBounds(pSubPred1, pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  //Card_Term* pTerm1 = _pCard_Term(pCard_PredCmp->pGet(1));

  Card_Term* pTerm2 = _pCard_Term(p_cmpPred->pGet(2));
  if (PB_NONE == E_eTerm_Bound(pTerm2))
    return false;

  Card_Term* pTerm0 = _pCard_Term(p_cmpPred->pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* does predicate have some bound wrt given index?                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bPRED_HasBounds(Card_PRED* p_PRED, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eCard = p_PRED->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return T_bOrPred_HasBounds(_pCard_OrPred(p_PRED), pIndex);

    case CK_ANDPRED:
      return T_bAndPred_HasBounds(_pCard_AndPred(p_PRED), pIndex);

    case CK_CMPPRED:
      return T_bCmpPred_HasBounds(_pCard_CmpPred(p_PRED), pIndex);

    default:
      vTrace("T_bPRED_HasBounds: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bPRED_IsPred(Card* pArgCard)
{
  _static_;

  eCK eCard = pArgCard->eCard();

  switch (eCard) {
    case CK_EXPRBOOL:
    case CK_ORPRED:
    case CK_ANDPRED:
      return true;

    default: return false;
  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Xtest::T_pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex,
                                          ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eCard = p_PRED->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return T_pOrPred_FindBound(_pCard_OrPred(p_PRED), pIndex, eBound);

    case CK_ANDPRED:
      return T_pAndPred_FindBound(_pCard_AndPred(p_PRED), pIndex, eBound);

    case CK_CMPPRED:
      return T_pCmpPred_FindBound(_pCard_CmpPred(p_PRED), pIndex, eBound);

    default:
      vTrace("T_pPRED_FindBound: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Xtest::T_pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_andPred->pGet(0);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)p_andPred->pGet(1);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Xtest::T_pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_orPred->pGet(0);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)p_orPred->pGet(1);
  if (Card_CmpPred* pFound = T_pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bBounds_Is_lbound(Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* p_term_0 = p_bounds->pSub_term();

  Stz* szBound = p_term_0->szValue();

  if (szBound->bSame("LBOUND"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bBounds_Is_lbound_Or_ubound(Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* p_term_0 = p_bounds->pSub_term();

  Stz* szBound = p_term_0->szValue();

  if (szBound->bSame("LBOUND"_Stz) || szBound->bSame("UBOUND"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bBounds_Is_ubound(Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* p_term_0 = p_bounds->pSub_term();

  Stz* szBound = p_term_0->szValue();

  if (szBound->bSame("UBOUND"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_Contains_Sqy_Call(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  CardIter xCI(p_cmpPred);
  while (Card* p_card = xCI.pEachChild()) {
    if (p_card->bIsCard(CK_TERM) &&
        T_bValue_IsSubQueryName(p_card->szValue()))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 =
      _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  char cTableSyn_this_0 = szColumnRef_this_0->cTableSyn();

  Card_ColumnRef* p_columnRef_this_1 =
      _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  Stz* szColumnRef_this_1 = p_columnRef_this_1->sz();
  char cTableSyn_this_1 = szColumnRef_this_1->cTableSyn();


  Card_ColumnRef* p_columnRef_arg_0 =
      _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  Stz* szColumnRef_arg_0 = p_columnRef_arg_0->sz();
  char cTableSyn_arg_0 = szColumnRef_arg_0->cTableSyn();

  Card_ColumnRef* p_columnRef_arg_1 =
      _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  Stz* szColumnRef_arg_1 = p_columnRef_arg_1->sz();
  char cTableSyn_arg_1 = szColumnRef_arg_1->cTableSyn();

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
/* find a comparePred which is the E_eTerm_Bound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Xtest::T_pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Term* pTerm2 = _pCard_Term(p_cmpPred->pGet(2));
  if (eBound != E_eTerm_Bound(pTerm2))
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(p_cmpPred->pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;

  return _pCard_CmpPred(this);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  Stz* szColumnRef_this_1 = p_columnRef_this_1->sz();

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  Stz* szColumnRef_arg_0 = p_columnRef_arg_0->sz();
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  Stz* szColumnRef_arg_1 = p_columnRef_arg_1->sz();

  if (szColumnRef_this_0->bSame(szColumnRef_arg_0) &&
      szColumnRef_this_1->bSame(szColumnRef_arg_1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_MaybeBounds(Card_CmpPred* p_cmpPred, Card_Table* pTable,
                                   bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  char cTblSyn = pTable->cValue();

  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (T_bValue_IsTableColOrdinal(pTerm_0->szValue())) {

    if (T_bValue_IsStreamColOrdinal(pTerm_1->szValue()))
      return true;

    char cTblSyn_0 = E_cValue_TableColSyn(pTerm_0->szValue());

    if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_1 = E_cValue_TableColSyn(pTerm_1->szValue());

      if (cTblSyn_0 == cTblSyn && cTblSyn_1 != cTblSyn)
        return true;
      else
      if (cTblSyn_1 == cTblSyn && cTblSyn_0 != cTblSyn)
        return true;
      else
        return false;
    }

    if (bParamAllowedInBounds &&
        cTblSyn_0 == cTblSyn &&
        T_bValue_IsParamOrdinal(pTerm_1->szValue()))
      return true;
    if (cTblSyn_0 == cTblSyn && T_bValue_IsResultOrdinal(pTerm_1->szValue()))
      return true;
  }

  if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_1 = E_cValue_TableColSyn(pTerm_1->szValue());

    if (bParamAllowedInBounds &&
        cTblSyn_1 == cTblSyn  &&
        T_bValue_IsParamOrdinal(pTerm_0->szValue()))
      return true;
    if (cTblSyn_1 == cTblSyn && T_bValue_IsResultOrdinal(pTerm_0->szValue()))
      return true;
  }
  else
  if (T_bValue_IsStreamColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_0 = E_cValue_TableColSyn(pTerm_0->szValue());
    if (bParamAllowedInBounds &&
        cTblSyn_0 == cTblSyn  &&
        T_bValue_IsParamOrdinal(pTerm_0->szValue()))
      return true;
    if (cTblSyn_0 == cTblSyn && T_bValue_IsResultOrdinal(pTerm_0->szValue()))
      return true;
  }

  return false;
}
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_MaybeBounds_v2(Card_CmpPred* p_cmpPred,
                                      Card_Table* p_table,
                                      bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: make this recursive

  Card_Index* p_index = p_table->pSub_index();

  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (T_bValue_IsTableColOrdinal(pTerm_0->szValue())) {
    // + term_0 is $c.n

    if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (T_bIndex_tblColOdn_IsKeyIn_index(pTerm_0->szValue(), p_index) ||
          T_bIndex_tblColOdn_IsKeyIn_index(pTerm_1->szValue(), p_index) )
        return true;

      return false;
    }
    else {
      // + term_0 is $c.n

      // ? check term_1 is permitted param
      if ((! bParamAllowedInBounds) &&
          T_bValue_IsParamOrdinal(pTerm_1->szValue()))
        return false;

      // ? is term_0:$c.n an index column
      if (T_bIndex_tblColOdn_IsKeyIn_index(pTerm_0->szValue(), p_index))
        return true;

      return false;
    }
  }
  else
  if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
    // + term_1 is $c.n

    // ? maybe term_0 is permitted param
    if ((! bParamAllowedInBounds) &&
        T_bValue_IsParamOrdinal(pTerm_0->szValue()))
      return false;

    // ? is term_0:$c.n an index column
    if (T_bIndex_tblColOdn_IsKeyIn_index(pTerm_1->szValue(), p_index) )
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
bool Xtest::T_bCmpPred_MaybeBoundsOf_index(Card_CmpPred* p_cmpPred,
                                           Card_Index* p_index,
                                           bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (T_bValue_IsTableColOrdinal(pTerm_0->szValue())) {
    // + term_0 is $c.n

    if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (T_bIndex_tblColOdn_IsKeyIn_index(pTerm_0->szValue(), p_index) ||
          T_bIndex_tblColOdn_IsKeyIn_index(pTerm_1->szValue(), p_index) )
        return true;

      // - neither term are a $c.n matching index column
      return false;
    }
    else {
      // + term_0 is $c.n

      // ? check term_1 is permitted param
      if ((! bParamAllowedInBounds) &&
          T_bValue_IsParamOrdinal(pTerm_1->szValue()))
        return false;

      // ? is term_0:$c.n an index column
      if (T_bIndex_tblColOdn_IsKeyIn_index(pTerm_0->szValue(), p_index))
        return true;

      // - term_0 is a $c.n but doesn't match index column
      return false;
    }
  }
  else
  if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
    // + term_1 is $c.n

    // ? maybe term_0 is permitted param
    if ((! bParamAllowedInBounds) &&
        T_bValue_IsParamOrdinal(pTerm_0->szValue()))
      return false;

    // ? is term_0:$c.n an index column
    if (T_bIndex_tblColOdn_IsKeyIn_index(pTerm_1->szValue(), p_index) )
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
/* is given cmpPred a subset of bounds?                                       */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_SubsetOf_bounds(Card_CmpPred* p_cmpPred, Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_cmpPred->vDump();

  CardIter xCI(p_bounds);
  while (Card* p_bound = xCI.pEachChild()) {

    p_bound->vDump();

    Card_Term* p_term_0 = _pCard_Term(p_bound->pGetChild(0));
    //Stz* szBound = p_term_0->szValue();

    Stz* sz_bnd_idxColOdn = p_bound->pGetChild(1)->szValue();
    Stz* sz_bnd_stmColOdn = p_bound->pGetChild(2)->szValue();
    Stz* sz_bnd_rlnOpr    = p_bound->pGetChild(3)->szValue();

    Stz* sz_cmp_rlnOpr    = p_cmpPred->szValue();
    Stz* sz_cmp_arg_0     = p_cmpPred->pGetChild(0)->szValue();
    Stz* sz_cmp_arg_1     = p_cmpPred->pGetChild(1)->szValue();

    if (! sz_cmp_rlnOpr->bSame(sz_bnd_rlnOpr))
      continue;

    // todo: need bounds to be bndTyp , etc
    // anyways...

    if (! sz_cmp_arg_1->bSame(sz_bnd_stmColOdn))
      continue;

    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  //this->vDump();
  //p_PredCmp_arg->vDump();

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  char cTableSyn_this_0 = E_cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  char cTableSyn_this_1 = E_cColumnRef_TableSyn(p_columnRef_this_1);

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = E_cColumnRef_TableSyn(p_columnRef_arg_0);
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = E_cColumnRef_TableSyn(p_columnRef_arg_1);

  if (   cTableSyn_this_0 == cTableSyn_arg_0
         || cTableSyn_this_0 == cTableSyn_arg_1
         || cTableSyn_this_1 == cTableSyn_arg_0
         || cTableSyn_this_1 == cTableSyn_arg_1
      )
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get column ordinal of column ref                                           */
/*----------------------------------------------------------------------------*/
Card_Term* Xtest::E_pCmpPred_Term_param(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Term* p_term_0 =_pCard_Term(p_cmpPred->pGetChild(0));
  if (! p_term_0)
    return nullptr;

  if (T_bValue_IsParamOrdinal(p_term_0->szValue()))
    return p_term_0;

  Card_Term* p_term_1 =_pCard_Term(p_cmpPred->pGetChild(1));
  if (! p_term_1)
    return nullptr;

  if (T_bValue_IsParamOrdinal(p_term_1->szValue()))
    return p_term_1;

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  char cTableSyn_this_0 = E_cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  char cTableSyn_this_1 = E_cColumnRef_TableSyn(p_columnRef_this_1);

  if (cTableSyn_this_0 == cTableSyn || cTableSyn_this_1 == cTableSyn)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* back to stream column ordinal $n                                           */
/*----------------------------------------------------------------------------*/
i8 Xtest::T_iCard_ConvertableTo_StmColOdn(Card* p_card)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eCK eCard = p_card->eCard();

  switch (eCard) {

    case CK_TERM: return C_YES;

    default:      return C_MAYBE;
  }
}

/*----------------------------------------------------------------------------*/
/* to table column ordinal $c.n                                               */
/*----------------------------------------------------------------------------*/
i8 Xtest::T_iCard_ConvertableTo_TblColOdn(Card* p_card)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eCK eCard = p_card->eCard();

  switch (eCard) {

    case CK_TERM: return C_YES;

    default:      return C_MAYBE;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCombined_Reinstate(eCK eMain, eCK eSubsequent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (CK_INDEXJOIN == eMain && (CK_AGGREG  == eSubsequent ||
                                CK_FILTER  == eSubsequent ||
                                CK_PROJECT == eSubsequent))
    return false;

  if (CK_AGGREG    == eMain ||
      CK_FILTER    == eMain ||
      CK_HAVING    == eMain ||
      CK_INDEXSCAN == eMain ||
      CK_INDEXJOIN == eMain ||
      CK_KEYEXISTS == eMain ||
      CK_PROJECT   == eMain ||
      CK_SORT      == eMain )
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_CmpPred* p_predCmp_this = _pCard_CmpPred(this);
  Card_CmpPred* p_predCmp_arg  = _pCard_CmpPred(p_CoPtnPred);

  if (T_bCmpPred_CoPtnWith(p_predCmp_this, p_predCmp_arg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin* Xtest::T_pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = p_coPtnJoinList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(p_coPtnJoinList->pGetChild(i_Child));

    if (T_bCoPtnJoin_CoPtnWith(p_coPtnJoin, p_coPtnPred))
      return p_coPtnJoin;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren_coPtn = p_coPtnJoinList->nChildCards();
  for (i8 i_Child_coPtn = 0; i_Child_coPtn < nChildren_coPtn; i_Child_coPtn++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(p_coPtnJoinList->pGetChild(i_Child_coPtn));

    i8 nChildren_predCmp = p_coPtnJoin->nChildCards();
    for (i8 i_Child_predCmp = 0; i_Child_predCmp < nChildren_predCmp; i_Child_predCmp++) {
      Card_CmpPred *p_predCmp = _pCard_CmpPred(p_coPtnJoin->pGetChild(i_Child_predCmp));


      if (T_bCmpPred_IsEq(p_predCmp, p_predCmp))
        return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nPtnJoins_sofar = pPriorPtnJoins_sofar->nChildCards();
  if (! nPtnJoins_sofar)
    return true;

  for (i8 i_PtnJoin_sofar = 0; i_PtnJoin_sofar < nPtnJoins_sofar; i_PtnJoin_sofar++) {
    Card_PtndJoin* p_ptndJoin_sofar =
        _pCard_PtndJoin(pPriorPtnJoins_sofar->pGetChild(i_PtnJoin_sofar));

    i8 nPredCmps_sofar = p_ptndJoin_sofar->nChildCards();
    for (i8 i_PredCmp_sofar = 0; i_PredCmp_sofar < nPredCmps_sofar; i_PredCmp_sofar++) {
      Card_CmpPred* p_predCmp_sofar =
          _pCard_CmpPred(p_ptndJoin_sofar->pGetChild(i_PredCmp_sofar));

      i8 nPredCmps_this = p_ptndJoin->nChildCards();
      for (i8 i_PredCmp_this = 0; i_PredCmp_this < nPredCmps_this; i_PredCmp_this++) {
        Card_CmpPred *p_predCmp_this =
            _pCard_CmpPred(p_ptndJoin->pGetChild(i_PredCmp_this));

        if (T_bCmpPred_OverlapsWith(p_predCmp_this, p_predCmp_sofar))
          return true;
      }

    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bStage_IsIgnorable(Card_Stage* p_stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Topology* p_topology = p_stage->pSub_topology();

  if (! p_topology)
    return false;

  if (YG_ALL_PARTITIONS == p_topology->Y_iGet(P_StageTopo))
    return false;

  return true;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bStage_IsRedundant(Card_Stage* p_stage, Card_Stage* p_prior)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! p_prior)
    return false;

  if (p_stage->bEquivalentTo(p_prior))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bStage_Reinstate(eCK eMain, eCK eSubsequent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (CK_STAGE     == eMain || eSubsequent == CK_STAGE)
    return false;

  if (CK_FILTER    == eMain && eSubsequent == CK_INDEXJOIN)
    return false;

  if (CK_INDEXJOIN == eMain && (eSubsequent == CK_PROJECT ||
                                eSubsequent == CK_FILTER))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bStage_ReinstateV2(eCK eMain, eCK eSubsequent, eAL eLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (CK_STAGE     == eMain || eSubsequent == CK_STAGE)
    return false;

  if (CK_FILTER    == eMain && eSubsequent == CK_INDEXJOIN)
    return false;

  if (CK_INDEXJOIN == eMain && (eSubsequent == CK_PROJECT ||
                                eSubsequent == CK_FILTER))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bStage_ReinstateV2(eAL& reLevel, Card* p_Main, Card* p_PriorToMain)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eMain  = p_Main->eCard();
  eCK ePrior = p_PriorToMain ? p_PriorToMain->eCard() : CK_NONE;

  if (CK_STAGE == eMain)
    return false;

  eAL eLevel_main = reLevel;
  Card_Level* p_level = p_Main->pSub_level();
  if (p_level)
    eLevel_main = (eAL) p_level->Y_iGet(P_Level);

  if ((CK_FILTER == eMain || CK_WHERE == eMain) && CK_AGGREG != ePrior) {
    // filter can be at P/N/O
    return false;
  }

  if (CK_INDEXJOIN == eMain) {
    reLevel = AL_PARTITION;
    return true;
  }

  if (CK_SCAN == eMain) {
    reLevel = AL_PARTITION;
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_AGGREG == eMain || CK_HAVING == eMain || CK_PROJECT == eMain) {
    if (eLevel_main == reLevel) {
//      if (CK_HAVING == ePrior)
//        vDebugPoint();
      if (CK_AGGREG == ePrior    || CK_FILTER == ePrior || CK_HAVING == ePrior ||
          CK_KEYEXISTS == ePrior || CK_PROJECT == ePrior)
        return true;
      return false;
    }
    reLevel = eLevel_main;
    return true;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bStage_ReinstateV3(eAL& reLevel, Card* p_Main, Card* p_PriorToMain)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eMain  = p_Main->eCard();
  eCK ePrior = p_PriorToMain ? p_PriorToMain->eCard() : CK_NONE;

  if (CK_AGGREG == ePrior)
    return false;

  if (CK_FILTER == eMain || CK_WHERE == eMain)
    return false;


  if (CK_INDEXJOIN == eMain)
    return true;

  if (CK_SCAN == eMain)
    return true;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_StmCol* Xtest::T_pStmColList_Find_streamCol(Card_StmColList* p_stmColList,
                                                 Stz *szColNm,
                                                 i8& ri_StreamColNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_StmColList* pCard_StreamColList = p_stmColList->pSub_stmColList();

  Card_StmCol* pCard_streamCol = nullptr;

  i8 nSubCards = p_stmColList->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_stmColList->pGetChild(i_Child);

    if (CK_STMCOL != pSubCard->eCard())
      continue;

    //Card_As* pCard_as = pSubCard->pSub_as();
    if (pSubCard->bSame(szColNm)) {
      pCard_streamCol = _pCard_StmCol(pSubCard);
      ri_StreamColNo = i_Child;
      break;
    }
  }

  return pCard_streamCol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bTableList_PredCmpIsCoPtn(Card_TableList* p_tableList, Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_ColumnRef* p_columnRef_0 =
      _pCard_ColumnRef(p_predCmp->pGetChild(0));
  Stz* szPredColumnRef_0 = p_columnRef_0->sz();

  Card_ColumnRef* p_columnRef_1 =
      _pCard_ColumnRef(p_predCmp->pGetChild(1));
  Stz* szPredColumnRef_1 = p_columnRef_1->sz();

  char cTableSyn_0 = E_cColumnRef_TableSyn(p_columnRef_0);
  char cTableSyn_1 = E_cColumnRef_TableSyn(p_columnRef_1);

  if (StrLib::bIsUpperCase(cTableSyn_0) || StrLib::bIsUpperCase(cTableSyn_1))
    return true;

  Card_Table* p_table_0 = T_pTableList_FindTable(p_tableList, cTableSyn_0);
  Card_TablePtn* p_tablePtn_0 = p_table_0->pSub_tablePtn();
  Stz* szTablePtn_0 = p_tablePtn_0->sz();

  Card_Table* p_table_1 = T_pTableList_FindTable(p_tableList, cTableSyn_1);
  Card_TablePtn* p_tablePtn_1 = p_table_1->pSub_tablePtn();
  Stz* szTablePtn_1 = p_tablePtn_1->sz();

  if (szTablePtn_0->bSame(szPredColumnRef_0) && szTablePtn_1->bSame(szPredColumnRef_1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Xtest::T_pTableList_FindTable(Card_TableList* p_tableList, char cSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = p_tableList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = p_tableList->pGetChild(i_Child);
    if (CK_TABLE != pChildCard->eCard())
      continue;

    Card_Table* p_table = _pCard_Table(pChildCard);
    char cTableSyn = p_table->cValue();
    if (cTableSyn == cSyn)
      return p_table;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bFilter_IsEmpty(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_BoolCalc* pCard_calcBool =
      _pCard_BoolCalc(p_filter->pFindSubCard(CK_BOOLCALC));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bIndex_tblColOdn_IsKeyIn_index(Stz* szTblColOdn, Card_Index* p_index)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  ASSERT(T_bValue_IsTableColOrdinal(szTblColOdn));

  p_index->vDump();
  Card_IdxColList* p_idxColList = p_index->pSub_idxColList();

  CardIter xCI(p_idxColList);
  while (Card_IdxCol* p_idxCol = _pCard_IdxCol(xCI.pEachChild())) {

    // ? does n]idxCol:$c.n match given:$c.n
    if (p_idxCol->bSame(szTblColOdn))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* recursively test if given predicate is entirely MaybeBounds of given index */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bPRED_AllSubs_MaybeBoundsOf_index(Card_PRED* p_PRED,
                                                 Card_Index* p_index,
                                                 bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (p_PRED->bIsConjunction()) {
    // + conjunction

    // ! all child predicates of conjunction must be MaybeBounds of index
    CardIter xCI(p_PRED);
    while (Card_PRED* p_childPRED = (Card_PRED*)xCI.pEachChild()) {
      if (! T_bPRED_AllSubs_MaybeBoundsOf_index(p_childPRED, p_index, bParamAllowedInBounds))
        return false;
    }

    // + all child predicates of conjunction are MaybeBounds of index
    return true;
  }
  else
  if (p_PRED->bIsCard(CK_CMPPRED)) {
    // + cmpPred

    if (! T_bCmpPred_MaybeBoundsOf_index(_pCard_CmpPred(p_PRED),p_index,bParamAllowedInBounds))
      return false;

    // + cmpPred MaybeBounds of index
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_PRED* Xtest::T_pPRED_Containing_MaybeBoundsOf_index(Card_PRED* p_PRED,
                                                         Card_Index* p_index,
                                                         bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (p_PRED->bIsConjunction()) {
    // + conjunction

    // * test if all child predicates of conjunction are MaybeBounds of index
    bool bAllChildren = true;
    CardIter xCI(p_PRED);
    while (Card_PRED* p_childPRED = (Card_PRED*)xCI.pEachChild()) {
      if (! T_bPRED_AllSubs_MaybeBoundsOf_index(p_childPRED, p_index, bParamAllowedInBounds)) {
        bAllChildren = false;
        break;
      }
    }

    if (bAllChildren)
      // + all child predicates of given pred are MaybeBounds of index
      return p_PRED;

    // * descend to find first sub predicate of conjunction which contains MaybeBounds of index
    xCI.vRestart();
    while (Card_PRED* p_childPRED = (Card_PRED*)xCI.pEachChild()) {
      Card_PRED *pSubPRED = T_pPRED_Containing_MaybeBoundsOf_index(p_childPRED, p_index, bParamAllowedInBounds);
      if (pSubPRED)
        return pSubPRED;
    }
    return nullptr;
  }
  else
  if (p_PRED->bIsCard(CK_CMPPRED)) {
    if (T_bCmpPred_MaybeBoundsOf_index(_pCard_CmpPred(p_PRED), p_index, bParamAllowedInBounds))
      return p_PRED;
    return nullptr;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bTblColSyn_Same(char cTblSyn0, char cTblSyn1)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (cTblSyn0 == cTblSyn1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is term an index-column ordinal?  (i.e. $In)                               */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsIndexColOrdinal(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];

  if ('$' == acFirst
      && ('I' == acSecond || 'i' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a key ordinal?  (i.e. $Kn)                                        */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsKeyOrdinal(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];

  if ('$' == acFirst
      && ('K' == acSecond || 'k' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a parameter ordinal?  (i.e. $Pn)                                  */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsParamOrdinal(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];

  if ('%' == acFirst
      && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}
/*----------------------------------------------------------------------------*/
//bool Xtest::T_bValue_IsParamOrdinal(Stz* szValue)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
//
//  char acFirst  = szValue->azChar()[0];
//  char acSecond = szValue->azChar()[1];
//  char acThird  = szValue->azChar()[2];
//
//  if ('$' == acFirst && ('P' == acSecond || 'p' == acSecond)
//      && StrLib::bIsDigit(acThird))
//    return true;
//  else
//    return false;
//}

/*----------------------------------------------------------------------------*/
/* is value a result ordinal?  (i.e. #n)                                      */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsResultOrdinal(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];

  if ('#' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a subQuery name?                                                  */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsSubQueryName(BASE::Stz *szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];
  char acFourth = szValue->azChar()[3];

  if ('_' == acFirst
      && '_' == acSecond
      && 's' == acThird
      && 'u' == acFourth)
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* is value a subQuery-column ordinal?  (i.e. __n)                            */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsSqyColOrdinal(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];
  char acFourth = szValue->azChar()[3];

  if ('_' == acFirst
      && '_' == acSecond
      && StrLib::bIsDigit(acThird))
    return true;

  if ('_' == acFirst
      && '_' == acSecond
      && 'S' == acThird
      && 'Q' == acFourth)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a stream-column ordinal?  (i.e. $n)                               */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsStreamColOrdinal(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a table-column ordinal?  (i.e. $c.n)                              */
/*----------------------------------------------------------------------------*/
bool Xtest::T_bValue_IsTableColOrdinal(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];
  char acFourth = szValue->azChar()[3];

  if ('$' == acFirst
      && StrLib::bIsAlpha(acSecond)
      && '.' == acThird
      && StrLib::bIsDigit(acFourth))
    return true;
  else
    return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
