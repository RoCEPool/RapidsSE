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
: Card_SUB(eCard)
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nChildCards = nItems();
  for (i8 i_ChildCard = 0; i_ChildCard < nChildCards; i_ChildCard++) {
    Card* pCard_Child = pGet(i_ChildCard);
    if (! pCard_Child->bIsPred())
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
                                             eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = this->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      return _pCard_OrPred(this)->pFindBound_or(pIndex,eBound);
      
    case CK_ANDPRED:
      return _pCard_AndPred(this)->pFindBound_and(pIndex,eBound);
      
    case CK_CMPPRED:
      return _pCard_CmpPred(this)->pFindBound_compare(pIndex,eBound);

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
      
    case CK_CMPPRED:
      return _pCard_CmpPred(this)->bHasBounds_compare(pIndex);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
