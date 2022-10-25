/*============================================================================*/
/* Xlator.cpp --                                                              */
/*============================================================================*/

#include "Xlator.hpp"

using namespace STORE::PRED;
using namespace STORE::Term;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Term::Xlate_Term::Xlate_Term(eCN eClassNo, Bench* pBench)
    : Accessor(eClassNo, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Term::Xlate_Term::bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
/* get column ordinal of column ref                                           */
/*----------------------------------------------------------------------------*/
i8 Term::Xlate_Term::iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szColumnRef = p_columnRef->sz();

  char* azColumnOrd = (szColumnRef->azChar() + 3);

  i8 iColumnOrd = StrLib::iGleanInt(azColumnOrd);

  return iColumnOrd;
}

/*----------------------------------------------------------------------------*/
/* get table syn of column ref                                                */
/*----------------------------------------------------------------------------*/
char Term::Xlate_Term::cColumnRef_TableSyn(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Term::Xlate_Term::cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Term::Xlate_Term::vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  psTableSyn->vSprint("%c"_Stz, cColumnRef_TableSyn_x(p_columnRef));
}


/*----------------------------------------------------------------------------*/
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB Term::Xlate_Term::Xlate_Term::eTerm_Bound(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eRO eRelOp = this->eTerm_RelOp(p_term);

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected eTerm_RelOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value  (i.e. $In)                                        */
/*----------------------------------------------------------------------------*/
i8 Term::Xlate_Term::iTerm_IndexColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(bTerm_IsIndexColOrdinal(p_term));

  i8 i_Ord = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* is term an index-column ordinal?  (i.e. $In)                               */
/*----------------------------------------------------------------------------*/
bool Term::Xlate_Term::bTerm_IsIndexColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term a parameter ordinal?  (i.e. $Pn)                                   */
/*----------------------------------------------------------------------------*/
bool Term::Xlate_Term::bTerm_IsParamOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];

  if ('$' == acFirst && ('P' == acSecond || 'p' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term a stream-column ordinal?  (i.e. $n)                                */
/*----------------------------------------------------------------------------*/
bool Term::Xlate_Term::bTerm_IsStreamColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term an table-column ordinal?  (i.e. $$c.n)                             */
/*----------------------------------------------------------------------------*/
bool Term::Xlate_Term::bTerm_IsTableColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];
  char acFourth = szTerm->azChar()[3];
  char acFifth  = szTerm->azChar()[4];

  if ('$' == acFirst && '$' == acSecond
      && StrLib::bIsAlpha(acThird)
      && '.' == acFourth
      && StrLib::bIsDigit(acFifth))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Term::Xlate_Term::eTerm_RelOp(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->sz();

  if (szTerm->bSame("<"_Stz))   return RO_LESS_THAN;
  else if (szTerm->bSame("<="_Stz))  return RO_LESS_EQUAL;
  else if (szTerm->bSame("="_Stz))   return RO_EQUAL_TO;
  else if (szTerm->bSame("=="_Stz))  return RO_EQUAL_TO;
  else if (szTerm->bSame("!="_Stz))  return RO_NOT_EQUAL;
  else if (szTerm->bSame(">="_Stz))  return RO_GREATER_EQUAL;
  else if (szTerm->bSame(">"_Stz))   return RO_GREATER_THAN;

  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* get stream column number n  (i.e. $n)                                      */
/*----------------------------------------------------------------------------*/
i8 Term::Xlate_Term::iTerm_StreamColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(bTerm_IsStreamColOrdinal(p_term));

  i8 i_Col = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table column number n (i.e. $$c.n)                                     */
/*----------------------------------------------------------------------------*/
i8 Term::Xlate_Term::iTerm_TableColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(bTerm_IsTableColOrdinal(p_term));

  i8 i_Col = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $$c.n)                                           */
/*----------------------------------------------------------------------------*/
char Term::Xlate_Term::cTerm_TableSyn(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(bTerm_IsTableColOrdinal(p_term));

  Stz* szTerm = p_term->sz();

  char cSyn = szTerm->azChar()[2];

  return cSyn;
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Term::Xlate_Term::eTerm_TermEval(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szTerm = p_term->sz();

  if (szTerm->bSame("and"_Stz))     return TE_AND;
  else if (szTerm->bSame("exists"_Stz))  return TE_EXISTS;
  else if (szTerm->bSame("or"_Stz))      return TE_OR;

  else if (szTerm->bSame("<"_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("<="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("="_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("=="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("!="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("<>"_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">"_Stz))       return TE_COMPARE;

  else return TE_OTHER;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PRED::Xlate_PRED::Xlate_PRED(eCN eClassNo, Bench* pBench)
    : Xlate_Term(eClassNo, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* concat terms                                                               */
/*----------------------------------------------------------------------------*/
void PRED::Xlate_PRED::vPRED_ConcatTerms(Card_PRED* p_PRED, Stz* szTerms, i8 i_From, i8 i_Till)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (C_UNKNOWN == i_Till)
    i_Till = p_PRED->nChildCards() - 1;

  for (i8 i_Child = i_From; i_Child <= i_Till; i_Child++) {
    Card* pChildCard = p_PRED->pGetChild(i_Child);
    if (CK_TERM != pChildCard->eCard())
      continue;

    szTerms->vCatSprint(" %s"_Stz, pChildCard->sz());
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PRED::Xlate_PRED::vPRED_CullByKids(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildCards = p_PRED->nItems();
  for (i8 i_ChildCard = 0; i_ChildCard < nChildCards; i_ChildCard++) {
    Card* pCard_Child = p_PRED->pGet(i_ChildCard);
    if (!bPRED_IsPred(pCard_Child))
      continue;

    Card_PRED* pCard_ChildPRED = (Card_PRED*)pCard_Child;

    vPRED_CullByKids(pCard_ChildPRED);

    if (0 == pCard_ChildPRED->nItems()) {

      p_PRED->vRemove(pCard_ChildPRED);
      delete pCard_ChildPRED;

      i_ChildCard--;
      nChildCards--;

      continue;
    }

    if (1 == pCard_ChildPRED->nItems()) {

      Card_PRED* pCard_KidPRED = (Card_PRED*)pCard_Child->pPop();

      p_PRED->vReplace(pCard_ChildPRED,pCard_KidPRED);
      delete pCard_ChildPRED;

      continue;
    }


  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* PRED::Xlate_PRED::pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex,
                                                ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eCard = p_PRED->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return pOrPred_FindBound(_pCard_OrPred(p_PRED), pIndex, eBound);

    case CK_ANDPRED:
      return pAndPred_FindBound(_pCard_AndPred(p_PRED), pIndex, eBound);

    case CK_CMPPRED:
      return pCmpPred_FindBound(_pCard_CmpPred(p_PRED), pIndex, eBound);

    default:
      vTrace("pPRED_FindBound: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does predicate have some bound wrt given index?                            */
/*----------------------------------------------------------------------------*/
bool PRED::Xlate_PRED::bPRED_HasBounds(Card_PRED* p_PRED, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eCard = p_PRED->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return bOrPred_HasBounds(_pCard_OrPred(p_PRED),pIndex);

    case CK_ANDPRED:
      return bAndPred_HasBounds(_pCard_AndPred(p_PRED),pIndex);

    case CK_CMPPRED:
      return bCmpPred_HasBounds(_pCard_CmpPred(p_PRED),pIndex);

    default:
      vTrace("bPRED_HasBounds: unexpected eCard %lld %s\n"_Stz
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
bool PRED::Xlate_PRED::bPRED_IsPred(Card* pArgCard)
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
/* find a comparePred which is the eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* PRED::Xlate_PRED::pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_andPred->pGet(0);
  if (Card_CmpPred* pFound = pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)p_andPred->pGet(1);
  if (Card_CmpPred* pFound = pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool PRED::Xlate_PRED::bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_andPred->pGet(0);
  if (bPRED_HasBounds(pSubPred0, pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)p_andPred->pGet(1);
  if (bPRED_HasBounds(pSubPred1, pIndex))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* PRED::Xlate_PRED::pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_orPred->pGet(0);
  if (Card_CmpPred* pFound = pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)p_orPred->pGet(1);
  if (Card_CmpPred* pFound = pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool PRED::Xlate_PRED::bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_PRED* pSubPred0 = (Card_PRED*)p_orPred->pGet(0);
  if (bPRED_HasBounds(pSubPred0,pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)p_orPred->pGet(1);
  if (bPRED_HasBounds(pSubPred1,pIndex))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool PRED::Xlate_PRED::bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool PRED::Xlate_PRED::bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
bool PRED::Xlate_PRED::bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //this->vDump();
  //p_PredCmp_arg->vDump();

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  char cTableSyn_this_0 = cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  char cTableSyn_this_1 = cColumnRef_TableSyn(p_columnRef_this_1);

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = cColumnRef_TableSyn(p_columnRef_arg_0);
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = cColumnRef_TableSyn(p_columnRef_arg_1);

  if (   cTableSyn_this_0 == cTableSyn_arg_0
         || cTableSyn_this_0 == cTableSyn_arg_1
         || cTableSyn_this_1 == cTableSyn_arg_0
         || cTableSyn_this_1 == cTableSyn_arg_1
      )
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* mark the bounds of this <predCmp>                                          */
/*----------------------------------------------------------------------------*/
void PRED::Xlate_PRED::vCmpPred_MarkBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szCmp = p_cmpPred->sz();

  char cBound = '?';

  if (szCmp->bSame("<"_Stz))  cBound = 'U';
  else if (szCmp->bSame("<="_Stz)) cBound = 'U';
  else if (szCmp->bSame("="_Stz))  cBound = 'E';
  else if (szCmp->bSame("=="_Stz)) cBound = 'E';
  else if (szCmp->bSame("!="_Stz)) cBound = 'N';
  else if (szCmp->bSame(">="_Stz)) cBound = 'L';
  else if (szCmp->bSame(">"_Stz))  cBound = 'L';

  p_cmpPred->vMark1(cBound);
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eTerm_Bound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* PRED::Xlate_PRED::pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_Term* pTerm2 = _pCard_Term(p_cmpPred->pGet(2));
  if (eBound != eTerm_Bound(pTerm2))
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(p_cmpPred->pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;

  return _pCard_CmpPred(this);
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool PRED::Xlate_PRED::bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //Card_Term* pTerm1 = _pCard_Term(pCard_PredCmp->pGet(1));

  Card_Term* pTerm2 = _pCard_Term(p_cmpPred->pGet(2));
  if (PB_NONE == eTerm_Bound(pTerm2))
    return false;

  Card_Term* pTerm0 = _pCard_Term(p_cmpPred->pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool PRED::Xlate_PRED::bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(p_cmpPred->pGetChild(0));
  char cTableSyn_this_0 = cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(p_cmpPred->pGetChild(1));
  char cTableSyn_this_1 = cColumnRef_TableSyn(p_columnRef_this_1);

  if (cTableSyn_this_0 == cTableSyn || cTableSyn_this_1 == cTableSyn)
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xlator::Xlator(eCN eClassNo, Bench* pBench)
: PRED::Xlate_PRED(eClassNo, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xlator::~Xlator(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull all <mainCard> == eCard from <board>                                  */
/*----------------------------------------------------------------------------*/
void Xlator::vCullAll(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  while (E_bEachMain(eCard)) {
    M.pDeck->vReplaceChildWithItsSubs(M.p_MainCard);
    DELETE_CARD(M.p_MainCard);
  }
}

/*----------------------------------------------------------------------------*/
/* cull all <mainCard> == eCard from <board>                                  */
/*----------------------------------------------------------------------------*/
void Xlator::vDeleteAll(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  while (E_bEachMain(eCard))
    M.xBI.vRemoveDeleteCurr(eCard);

  vRenumberMainCards();
}

/*----------------------------------------------------------------------------*/
/* renumber the main cards                                                    */
/*----------------------------------------------------------------------------*/
void Xlator::vRenumberMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //m_pBoard->vRenumberMainCards();

  M.pDeck->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Xlator::R_vReplaceSub(Card* pSub, Card* pInParent, Card* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pParent = pInParent->pFindParent(pSub);

  pParent->vReplaceChild(pSub,pWith);
}

/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Xlator::X_calc_To_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_AndPred*  p_andPred = nullptr;
  Card_CmpPred*  p_cmpPred = nullptr;
  Card_TstPred* p_tstPred = nullptr;
  Card_OrPred*   p_orPred  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  N_vNewMember(CK_BOOLCALC);

  while (M.p_rpnCalc->nChildCards()) {
    M.p_term = _pCard_Term(M.p_rpnCalc->pPop());

    eTE eTopTerm = eTerm_TermEval(M.p_term);
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == M.p_boolCalc->nChildCards());
        p_andPred = new Card_AndPred;
        p_andPred->vTransferChildren(M.p_boolCalc);
        RASSERT(0 == M.p_boolCalc->nChildCards());
        M.p_boolCalc->vAppend(p_andPred);
        RASSERT(1 == M.p_boolCalc->nChildCards());
        p_andPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_OR:
        RASSERT(2 == M.p_boolCalc->nChildCards());
        p_orPred = new Card_OrPred;
        p_orPred->vTransferChildren(M.p_boolCalc);
        RASSERT(0 == M.p_boolCalc->nChildCards());
        M.p_boolCalc->vAppend(p_orPred);
        RASSERT(1 == M.p_boolCalc->nChildCards());
        p_orPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nChildCards() > 0);
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vValue(M.p_term->sz());
        p_cmpPred->vTransferChildren(pCard_SubExpr);
        M.p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_EXISTS:
        p_tstPred = new Card_TstPred;
        p_tstPred->vValue(M.p_term->sz());
        p_tstPred->vTransferChildren(pCard_SubExpr);
        M.p_boolCalc->vAppend(p_tstPred);
        p_tstPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(M.p_term);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  //M.p_boolCalc->vDump();

  if (pCard_SubExpr->nItems()) {
    RASSERT(0 == M.p_boolCalc->nChildCards());
    p_cmpPred->vTransferChildren(pCard_SubExpr);
    RASSERT(1 == M.p_boolCalc->nChildCards());
    RASSERT(0 == pCard_SubExpr->nChildCards());
  }

  delete pCard_SubExpr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Xlator::pFind_table(Card_TableList* p_tableList, char cSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
bool Xlator::bPredCmpIsCoPtn(Card_TableList* p_tableList, Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_ColumnRef* p_columnRef_0 =
      _pCard_ColumnRef(p_predCmp->pGetChild(0));
  Stz* szPredColumnRef_0 = p_columnRef_0->sz();

  Card_ColumnRef* p_columnRef_1 =
      _pCard_ColumnRef(p_predCmp->pGetChild(1));
  Stz* szPredColumnRef_1 = p_columnRef_1->sz();

  char cTableSyn_0 = cColumnRef_TableSyn(p_columnRef_0);
  char cTableSyn_1 = cColumnRef_TableSyn(p_columnRef_1);

  if (StrLib::bIsUpperCase(cTableSyn_0) || StrLib::bIsUpperCase(cTableSyn_1))
    return true;

  Card_Table* p_table_0 = pFind_table(p_tableList, cTableSyn_0);
  Card_TablePtn* p_tablePtn_0 = p_table_0->pSub_tablePtn();
  Stz* szTablePtn_0 = p_tablePtn_0->sz();

  Card_Table* p_table_1 = pFind_table(p_tableList,cTableSyn_1);
  Card_TablePtn* p_tablePtn_1 = p_table_1->pSub_tablePtn();
  Stz* szTablePtn_1 = p_tablePtn_1->sz();

  if (szTablePtn_0->bSame(szPredColumnRef_0) && szTablePtn_1->bSame(szPredColumnRef_1))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Xlator::bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      vBoolCalc_RemovePred(p_boolCalc,p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      vBoolCalc_RemovePred(p_boolCalc,p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      vBoolCalc_RemovePred(p_boolCalc,p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (bBoolCalc_CullPred(p_boolCalc,pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xlator::vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (! p_boolCalc->nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  while (pCard_pred &&
         bBoolCalc_CullPred(p_boolCalc, pCard_pred, cWithMark) &&
         p_boolCalc->nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xlator::vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card* pParentCard = p_boolCalc->pFindParent(p_pred);

  pParentCard->vRemoveSubCard(p_pred);

  if (CK_ANDPRED == pParentCard->eCard() || CK_ORPRED == pParentCard->eCard()) {

    if (0 == pParentCard->nChildCards()) {
      p_boolCalc->vRemoveSubCard(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetChild(0);
      Card* pGrandCard = p_boolCalc->pFindParent((pParentCard));
      pParentCard->vRemoveSubCard(pSiblingCard);
      pGrandCard->vReplaceChild(pParentCard, pSiblingCard);
    }

    delete pParentCard;
  }
}

/*----------------------------------------------------------------------------*/
/* get rank                                                                   */
/*----------------------------------------------------------------------------*/
i8 Xlator::iCandIndex_Rank(Card_CandIndex* p_candIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iRank = p_candIndex->Y_iVal(P_Rank);

  return iRank;
}

/*----------------------------------------------------------------------------*/
/* set rank                                                                   */
/*----------------------------------------------------------------------------*/
void Xlator::vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  p_candIndex->Y_vSet(P_Rank, iRank);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_CmpPred* p_predCmp_this = _pCard_CmpPred(this);
  Card_CmpPred* p_predCmp_arg  = _pCard_CmpPred(p_CoPtnPred);

  if (bCmpPred_CoPtnWith(p_predCmp_this, p_predCmp_arg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin* Xlator::pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildren = p_coPtnJoinList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(p_coPtnJoinList->pGetChild(i_Child));

    if (bCoPtnJoin_CoPtnWith(p_coPtnJoin, p_coPtnPred))
      return p_coPtnJoin;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildren_coPtn = p_coPtnJoinList->nChildCards();
  for (i8 i_Child_coPtn = 0; i_Child_coPtn < nChildren_coPtn; i_Child_coPtn++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(p_coPtnJoinList->pGetChild(i_Child_coPtn));

    i8 nChildren_predCmp = p_coPtnJoin->nChildCards();
    for (i8 i_Child_predCmp = 0; i_Child_predCmp < nChildren_predCmp; i_Child_predCmp++) {
      Card_CmpPred *p_predCmp = _pCard_CmpPred(p_coPtnJoin->pGetChild(i_Child_predCmp));


      if (bCmpPred_IsEq(p_predCmp, p_predCmp))
        return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColList* Xlator::pELIST_DeriveStreamColList(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_StmColList* pCard_StreamColList = new Card_StmColList;

  eCK eSubCard;
  i8 nSubCards = p_ELIST->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_ELIST->pGetChild(i_Child, eSubCard);

    Card *pCard_Type = pSubCard->pFindSubCard(CK_TYPE);
    Card *pCard_As = pSubCard->pFindSubCard(CK_AS);

    Card_StmCol* pCard_StreamCol = new Card_StmCol;
    pCard_StreamColList->vAppend(pCard_StreamCol);

    if (pCard_As)
      pCard_StreamCol->vAppend(pCard_As->pDupCardAndSubCards());
    else {
      s32 sAs;
      sAs.vSprint("$%lld"_Stz,i_Child + 1);
      pCard_As = new Card_As(sAs);
      pCard_StreamCol->vAppend(pCard_As);
    }
    pCard_StreamCol->vAppend(pCard_Type->pDupCardAndSubCards());
  }

  return pCard_StreamColList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::vExpr_AsEnsure(Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_As* pCard_As = _pCard_As(p_expr->pFindSubCard(CK_AS));
  if (pCard_As)
    return;

  s32 sAs;
  sAs.vSprint("COL_%lld"_Stz,p_expr->iCardNo());

  pCard_As = new Card_As(sAs);
  p_expr->vAppend(pCard_As);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::vFilter_CullPred(Card_Filter* p_filter, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildren = p_filter->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = p_filter->pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_BOOLCALC)
      continue;

    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);

    vBoolCalc_CullPred(p_boolCalc,cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::bFilter_IsEmptyFilter(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_BoolCalc* pCard_calcBool =
      _pCard_BoolCalc(p_filter->pFindSubCard(CK_BOOLCALC));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Xlator::vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iOrdLo = pCard_Stream->Y_iVal(P_OrdLo);

  Card_StmColList* pCard_StreamColList = pCard_Stream->pSub_stmColList();

  i8 nSubCards = p_idxColList->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_IdxCol* pCard_idxCol = _pCard_IdxCol(p_idxColList->pGetChild(i_Sub));

    i8 i_StreamColNo = C_UNKNOWN;
    Stz* szColNm = pCard_idxCol->sz();
    Card_StmCol* pCard_streamCol =
        pStmColList_Find_streamCol(pCard_StreamColList, szColNm, i_StreamColNo);

    s32 sOrdinal;
    sOrdinal.vSprint("$%lld"_Stz,iOrdLo+i_StreamColNo);
    pCard_idxCol->vValue(sOrdinal);

    pCard_idxCol->vMark1(MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::vJoinOrderSeq_ExtractJoinOrder(Card_JoinOrderSeq* p_joinOrderSeq,
                                            Card_PtndJoin* p_PtndJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eChild;

  for (i8 i_Child = 0; i_Child < p_PtndJoin->nChildCards(); i_Child++) {
    Card_CmpPred* p_predCmp = _pCard_CmpPred(p_PtndJoin->pGetChild(i_Child));

    for (i8 i_Kid = p_predCmp->nChildCards() - 1; i_Kid >= 0; i_Kid--) {
      Card *pChild = p_predCmp->pGetChild(i_Kid, eChild);
      if (CK_COLUMNREF != eChild)
        continue;

      Card_ColumnRef* p_columnRef = _pCard_ColumnRef(pChild);
      s32 sTableSyn;
      vColumnRef_TableSyn_x(p_columnRef,sTableSyn);

      if (p_joinOrderSeq->bHasChildWithValue(sTableSyn))
        continue;

      Card_JoinOrder* p_joinOrder = new Card_JoinOrder(sTableSyn);
      p_joinOrderSeq->vAppend(p_joinOrder);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  auto bfExtract = [this,p_joinPredList,p_boolCalc](Card *pCard) {

    Card_CmpPred* p_predCmp = _pCard_CmpPred(pCard);

    if (! p_predCmp->sz()->bSame("=="_Stz)) //RAYMOND CAN DO BETTER!
      return true;

    Card_Term* p_term_0 = _pCard_Term(p_predCmp->pGetChild(0));
    Card_ColumnRef* p_columnRef_0 = p_term_0->pSub_columnRef();
    if (! p_columnRef_0)
      return true;
    Card_Term* p_term_1 = _pCard_Term(p_predCmp->pGetChild(1));
    Card_ColumnRef* p_columnRef_1= p_term_1->pSub_columnRef();
    if (! bColumnRef_IsColumnRef(p_columnRef_1))
      return true;

    Card_CmpPred* p_predCmp_new = new Card_CmpPred(p_predCmp->sz());
    p_predCmp_new->vAppend(p_columnRef_0->pDupCardAndSubCards());
    p_predCmp_new->vAppend(p_columnRef_1->pDupCardAndSubCards());

    p_joinPredList->vAppend(p_predCmp_new);

    return true;
  };

  // - - - - - - - - -

  p_boolCalc->vApplyRecursive(CK_CMPPRED, bfExtract);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();

  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {

    Column* pIdxCol = pIndexDef->pGetColumn(i_IdxCol);

    Card_Expr* pCard_Expr = new Card_Expr;
    p_obyElist->vAppend(pCard_Expr);


    Stz* szType = aCT[pIdxCol->eColumnType()];
    Card_Type* pCard_Type = new Card_Type(szType);
    pCard_Expr->vAppend(pCard_Type);

    // as
    Card_As* pCard_As = new Card_As(pIdxCol->szColNm());
    pCard_Expr->vAppend(pCard_As);

    // exprCalc
    Card_RpnCalc* pCard_RpnCalc = new Card_RpnCalc;
    pCard_Expr->vAppend(pCard_RpnCalc);

    Card_Term* pCard_Term = new Card_Term(pIdxCol->szColNm());
    pCard_RpnCalc->vAppend(pCard_Term);

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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

        if (bCmpPred_OverlapsWith(p_predCmp_this,p_predCmp_sofar))
          return true;
      }

    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nJoinedBuff = 0;

  i8 nTables = p_tableList->nChildCards();
  for (i8 i_Table = 0; i_Table < nTables; i_Table++) {
    Card_Table *p_table =
        _pCard_Table(p_tableList->pGetChild(i_Table));

    if (p_table->bHasProperty(P_TableUsed))
      continue;

    i8 nPredCmps = p_ptndJoin->nChildCards();
    for (i8 i_PredCmp = 0; i_PredCmp < nPredCmps; i_PredCmp++) {
      Card_CmpPred *p_predCmp =
          _pCard_CmpPred(p_ptndJoin->pGetChild(i_PredCmp));

      if (bCmpPred_UsesTable(p_predCmp,p_table->cValue())) {
        nJoinedBuff += p_table->pSub_size()->szValue()->iToI8();
        p_table->Y_vSet(P_TableUsed);
      }
    }

  }

  p_ptndJoin->Y_vSet(P_JoinedBuff, nJoinedBuff);
}


/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_StmCol* Xlator::pStmColList_Find_streamCol(Card_StmColList* p_stmColList,
                                                Stz *szColNm,
                                                i8& i_StreamColNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
      i_StreamColNo = i_Child;
      break;
    }
  }

  return pCard_streamCol;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
