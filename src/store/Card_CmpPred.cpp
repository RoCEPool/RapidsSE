/*============================================================================*/
/* Card_CmpPred.cpp -- Compare Predicate                                      */
/*============================================================================*/

#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CmpPred,CK_CMPPRED)
__aNewFunction(Card_CmpPred,CN_CARD)
__vDeleteFunction(Card_CmpPred,CN_CARD)

__bIsCard(STORE,Card_CmpPred,CK_CMPPRED)

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
/* get bound of this compare op                                               */
/*----------------------------------------------------------------------------*/
eBK Card_CmpPred::eBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 i_LastCard = nItems() - 1;
  Card* pLastCard = pGet(i_LastCard);

  eBK eBound = pLastCard->eBound();
  
  return eBound;
}

/*----------------------------------------------------------------------------*/
/* get relational operator of this compare op                                 */
/*----------------------------------------------------------------------------*/
eRO Card_CmpPred::eRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 i_LastCard = nItems() - 1;
  Card* pLastCard = pGet(i_LastCard);

  eRO eRelOp = pLastCard->eRelOp();
  
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

    Stz* szTerm = pCard_Term->szTerm();

    pComparePred->vAppend(new Card_Term(szTerm));
  }
  
  return pComparePred;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_CmpPred::pFindBound_compare(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Term* pTerm2 = _pCard_Term(pGet(2));
  if (eBound != pTerm2->eBound())
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(pGet(0));
  Stz* szTerm0 = pTerm0->szTerm();
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
  
  //Card_Term* pTerm1 = _pCard_Term(pCard_CmpPred->pGet(1));
  
  Card_Term* pTerm2 = _pCard_Term(pGet(2));
  if (BK_NONE == pTerm2->eBound())
    return false;

  Card_Term* pTerm0 = _pCard_Term(pGet(0));
  Stz* szTerm0 = pTerm0->szTerm();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;
  
  return true;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
