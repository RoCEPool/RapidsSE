/*============================================================================*/
/* Card_CmpPred.cpp -- Compare Predicate                                      */
/*============================================================================*/

#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_ColumnRef.hpp"

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
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bCoPtnWith(Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 =
      _pCard_ColumnRef(this->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  char cTableSyn_this_0 = szColumnRef_this_0->cTableSyn();

  Card_ColumnRef* p_columnRef_this_1 =
      _pCard_ColumnRef(this->pGetChild(1));
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
bool Card_CmpPred::bIsEq(Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(this->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(this->pGetChild(1));
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
bool Card_CmpPred::bOverlapsWith(Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  this->vDump();
  p_PredCmp_arg->vDump();

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(this->pGetChild(0));
  char cTableSyn_this_0 = p_columnRef_this_0->cColumnRef_tableSyn();
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(this->pGetChild(1));
  char cTableSyn_this_1 = p_columnRef_this_1->cColumnRef_tableSyn();

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_columnRef_arg_0->cColumnRef_tableSyn();
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_columnRef_arg_1->cColumnRef_tableSyn();

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
void Card_CmpPred::vMarkBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szCmp = this->sz();

  char cBound = '?';

       if (szCmp->bSame("<"_Stz))  cBound = 'U';
  else if (szCmp->bSame("<="_Stz)) cBound = 'U';
  else if (szCmp->bSame("="_Stz))  cBound = 'E';
  else if (szCmp->bSame("=="_Stz)) cBound = 'E';
  else if (szCmp->bSame("!="_Stz)) cBound = 'N';
  else if (szCmp->bSame(">="_Stz)) cBound = 'L';
  else if (szCmp->bSame(">"_Stz))  cBound = 'L';

  vMark1(cBound);
}





/*----------------------------------------------------------------------------*/
/* get bound of this compare op                                               */
/*----------------------------------------------------------------------------*/
ePB Card_CmpPred::eBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 i_LastCard = nItems() - 1;
  Card_Term* p_term_last = _pCard_Term(pGetChild(i_LastCard));

  ePB eBound = p_term_last->eBound();
  
  return eBound;
}

/*----------------------------------------------------------------------------*/
/* get relational operator of this compare op                                 */
/*----------------------------------------------------------------------------*/
eRO Card_CmpPred::eRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 i_LastCard = nItems() - 1;
  Card_Term* p_term_last = _pCard_Term(pGetChild(i_LastCard));

  eRO eRelOp = p_term_last->eRelOp();
  
  return eRelOp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_CmpPred::pDup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_CmpPred* pComparePred = new Card_CmpPred;
  
  i8 nTerms = this->nItems();
  for (i8 i_Term = 0; i_Term < nTerms; i_Term++) {
    Card_Term* pCard_Term = _pCard_Term(pGet(i_Term));

    Stz* szTerm = pCard_Term->sz();

    pComparePred->vAppend(new Card_Term(szTerm));
  }
  
  return pComparePred;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_CmpPred::pFindBound_compare(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Term* pTerm2 = _pCard_Term(pGet(2));
  if (eBound != pTerm2->eBound())
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bHasBounds_compare(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  //Card_Term* pTerm1 = _pCard_Term(pCard_PredCmp->pGet(1));
  
  Card_Term* pTerm2 = _pCard_Term(pGet(2));
  if (PB_NONE == pTerm2->eBound())
    return false;

  Card_Term* pTerm0 = _pCard_Term(pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bUsesTable(char cTableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(this->pGetChild(0));
  char cTableSyn_this_0 = p_columnRef_this_0->cColumnRef_tableSyn();
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(this->pGetChild(1));
  char cTableSyn_this_1 = p_columnRef_this_1->cColumnRef_tableSyn();

  if (cTableSyn_this_0 == cTableSyn || cTableSyn_this_1 == cTableSyn)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this <predCmp> usable as a <joinPred>                                  */
/*----------------------------------------------------------------------------*/
bool Card_CmpPred::bIsJoinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (sz())
    return false;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
