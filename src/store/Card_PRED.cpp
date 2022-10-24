/*============================================================================*/
/* Card_PRED.cpp -- Abstract Predicate                                        */
/*============================================================================*/

#include "Card_PRED.hpp"
#include "Card_Term.hpp"
#include "Card_OrPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_CmpPred.hpp"

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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PRED::Card_PRED(eCK eCard, Stz* szValue)
    : Card(eCard,szValue)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* concat terms                                                               */
/*----------------------------------------------------------------------------*/
void Card_PRED::vConcatTerms(Stz* szTerms, i8 i_From, i8 i_Till)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (C_UNKNOWN == i_Till)
    i_Till = nChildCards() - 1;

  for (i8 i_Child = i_From; i_Child <= i_Till; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);
    if (CK_TERM != pChildCard->eCard())
      continue;

    szTerms->vCatSprint(" %s"_Stz, pChildCard->sz());
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_PRED::vCullByKids(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nChildCards = nItems();
  for (i8 i_ChildCard = 0; i_ChildCard < nChildCards; i_ChildCard++) {
    Card* pCard_Child = pGet(i_ChildCard);
    if (! bIsPred(pCard_Child))
      continue;
    
    Card_PRED* pCard_ChildPRED = (Card_PRED*)pCard_Child;
    
    pCard_ChildPRED->vCullByKids();
    
    if (0 == pCard_ChildPRED->nItems()) {
    
      vRemove(pCard_ChildPRED);
      delete pCard_ChildPRED;
    
      i_ChildCard--;
      nChildCards--;

      continue;
    }
    
    if (1 == pCard_ChildPRED->nItems()) {

      Card_PRED* pCard_KidPRED = (Card_PRED*)pCard_Child->pPop();

      vReplace(pCard_ChildPRED,pCard_KidPRED);
      delete pCard_ChildPRED;
    
      continue;
    }
    
    
  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_PRED::pFindBound_pred(Index* pIndex,
                                         ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = this->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      return _pCard_OrPred(this)->pOrPred_FindBound(_pCard_OrPred(this), pIndex, eBound);
      
    case CK_ANDPRED:
      return _pCard_AndPred(this)->pAndPred_FindBound(_pCard_AndPred(this), pIndex, eBound);
      
    case CK_CMPPRED:
      return _pCard_CmpPred(this)->pCmpPred_FindBound(_pCard_CmpPred(this), pIndex, eBound);

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
      return _pCard_OrPred(this)->bOrPred_HasBounds(_pCard_OrPred(this),pIndex);
      
    case CK_ANDPRED:
      return _pCard_AndPred(this)->bAndPred_HasBounds(_pCard_AndPred(this),pIndex);
      
    case CK_CMPPRED:
      return _pCard_CmpPred(this)->bCmpPred_HasBounds(_pCard_CmpPred(this),pIndex);

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
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bIsPred(Card* pArgCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = pArgCard->eCard();

  switch (eCard) {
    case CK_EXPRBOOL:
    case CK_ORPRED:
    case CK_ANDPRED:
      return true;

    default: return false;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
