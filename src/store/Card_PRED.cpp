/*============================================================================*/
/* Card_PRED.cpp -- Abstract Predicate                                        */
/*============================================================================*/

#include "Card_PRED.hpp"
#include "Card_Term.hpp"
#include "Card_OrPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_ComparePred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PRED::Card_PRED(eCK eCard)
: Card(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PRED::~Card_PRED(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_PRED::vCullByKids(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  i8 nChildCards = nItems();
  for (i8 i_ChildCard = 0; i_ChildCard < nChildCards; i_ChildCard++) {
    Card* pChild = pGet(i_ChildCard);
    if (! pChild->bIsPred())
      continue;
    
    Card_PRED* pChildPRED = (Card_PRED*)pChild;
    
    pChildPRED->vCullByKids();
    
    if (0 == pChildPRED->nItems()) {
    
      vRemove(pChildPRED);
      delete pChildPRED;
    
      i_ChildCard--;
      nChildCards--;

      continue;
    }
    
    if (1 == pChildPRED->nItems()) {

      Card_PRED* pKidPRED = (Card_PRED*)pChild->pPop();

      vReplace(pChildPRED,pKidPRED);
      delete pChildPRED;
    
      continue;
    }
    
    
  }
}

/*----------------------------------------------------------------------------*/
/* replace any ordinal term with stream column name                           */
/*----------------------------------------------------------------------------*/
void Card_PRED::vReplaceOrdinals(StreamDef* pStreamDef, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* test for atom card and replace a column ordinal or an index ordinal    */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vReplaceAnOrdinal = [pStreamDef,pIndex](Card* pCard)
  {
    //vTrace("vReplaceOrdinals: %s\n"_Stz,pCard->szCardNm());

    if (CK_TERM != pCard->eCard())
      return;

    Card_Term* pCard_Term = _pCard_Term(pCard);
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // an ordinal can appear as a column ordinal
  
    if (pCard_Term->bIsOrdinalColumnTerm()) {

      i8 i_Ord = pCard_Term->iOrdinalColumnValue();
      i8 i_Col = i_Ord - 1;

      Column* pColumn = pStreamDef->pGetColumn(i_Col);
      Stz* szColNm = pColumn->szColNm();

      pCard_Term->vAddProperty("szTerm"_Stz,szColNm);

      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or as an index ordinal
  
    if (pIndex && pCard_Term->bIsOrdinalIndexTerm()) {
      
      i8 i_IdxOrd = pCard_Term->iOrdinalIndexValue();
      i8 i_IdxCol = i_IdxOrd - 1;

      Column* pIdxColumn = pIndex->pGetColumn(i_IdxCol);
      Stz* szIdxColNm = pIdxColumn->szColNm();
    
      pCard_Term->vAddProperty("szTerm"_Stz,szIdxColNm);

      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise the atom is not a column reference
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // walk all subcards of predicate, replacing columns ordinals with names
  //

  vEachSubCard(vReplaceAnOrdinal);
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Card_PRED::pFindBound_pred(Index* pIndex,
                                             eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = this->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      return _pCard_OrPred(this)->pFindBound_or(pIndex,eBound);
      
    case CK_ANDPRED:
      return _pCard_AndPred(this)->pFindBound_and(pIndex,eBound);
      
    case CK_COMPAREPRED:
      return _pCard_ComparePred(this)->pFindBound_compare(pIndex,eBound);

    default:
      vTrace("pFindBound_pred: unexpected eCard %lld %s\n"_Stz
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
bool Card_PRED::bHasBounds_pred(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = this->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      return _pCard_OrPred(this)->bHasBounds_or(pIndex);
      
    case CK_ANDPRED:
      return _pCard_AndPred(this)->bHasBounds_and(pIndex);
      
    case CK_COMPAREPRED:
      return _pCard_ComparePred(this)->bHasBounds_compare(pIndex);

    default:
      vTrace("bHasBounds_pred: unexpected eCard %lld %s\n"_Stz
             ,(i8)eCard
             ,aCK[eCard]);
      RASSERT(0);
      break;
  }
  

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove child pred from this predicate                                      */
/*----------------------------------------------------------------------------*/
void Card_PRED::vReducePred(Card_PRED* pCard_PRED_child)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  this->vDump();
  pCard_PRED_child->vDump();
  
  // the child will only have one subcard
  i8 nChildSubCards = pCard_PRED_child->nItems();
  RASSERT(1 == nChildSubCards);
  
  //pCard_PRED_parent->vRemove(pCard_PRED_child);
  //pCard_PRED_parent->vDump();
  
  Card_PRED* pCard_PRED_kid = (Card_PRED*)pCard_PRED_child->pPop();
  pCard_PRED_kid->vDump();

  this->vReplace(pCard_PRED_child,pCard_PRED_kid);
  this->vDump();

  delete pCard_PRED_child;
  
  this->vDump();

}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bReduciblePred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = this->eCard();
  i8 nSubCards = nItems();
  
  if (1 == nSubCards && (CK_ORPRED == eCard || CK_ANDPRED == eCard))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* remove given comparePred                                                   */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bRemoveBound_pred(Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  eCK eCard = this->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      if (_pCard_OrPred(this)->bRemoveBound_or(pCard_ComparePred))
        return true;
      
    case CK_ANDPRED:
      if (_pCard_AndPred(this)->bRemoveBound_and(pCard_ComparePred))
        return true;
      
    default:
      vTrace("vCardPred: unexpected eCard %lld %s\n"_Stz
             ,(i8)eCard
             ,aCK[eCard]);
      RASSERT(0);
      break;
  }
  
  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
