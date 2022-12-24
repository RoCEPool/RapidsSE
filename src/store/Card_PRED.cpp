/*============================================================================*/
/* Card_PRED.cpp -- Abstract Predicate                                        */
/*============================================================================*/

#include "Card_PRED.hpp"
#include "Card_Term.hpp"
#include "Card_OrPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_CmpPred.hpp"
#include "CardIter.hpp"

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
/* recursively test if given predicate is entirely MaybeBounds of given index */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bAllSubs_MaybeBoundsOf_index(Card_Index* p_index, bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (this->bIsConjunction()) {
    // + conjunction

    // ! all child predicates of conjunction must be MaybeBounds of index
    CardIter xCI(this);
    while (Card_PRED* p_childPRED = (Card_PRED*)xCI.pEachChild()) {
      if (! p_childPRED->bAllSubs_MaybeBoundsOf_index(p_index, bParamAllowedInBounds))
        return false;
    }

    // + all child predicates of conjunction are MaybeBounds of index
    return true;
  }
  else
  if (this->bIsCard(CK_CMPPRED)) {
    // + cmpPred

    if (! _pCard_CmpPred(this)->bMaybeBoundsOf_index(p_index,bParamAllowedInBounds))
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
Card_PRED* Card_PRED::pContaining_MaybeBoundsOf_index(Card_Index* p_index,
                                                         bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (this->bIsConjunction()) {
    // + conjunction

    // * test if all child predicates of conjunction are MaybeBounds of index
    bool bAllChildren = true;
    CardIter xCI(this);
    while (Card_PRED* p_childPRED = (Card_PRED*)xCI.pEachChild()) {
      if (! p_childPRED->bAllSubs_MaybeBoundsOf_index(p_index, bParamAllowedInBounds)) {
        bAllChildren = false;
        break;
      }
    }

    if (bAllChildren)
      // + all child predicates of given pred are MaybeBounds of index
      return this;

    // * descend to find first sub predicate of conjunction which contains MaybeBounds of index
    xCI.vRestart();
    while (Card_PRED* p_childPRED = (Card_PRED*)xCI.pEachChild()) {
      Card_PRED *pSubPRED = p_childPRED->pContaining_MaybeBoundsOf_index(p_index, bParamAllowedInBounds);
      if (pSubPRED)
        return pSubPRED;
    }
    return nullptr;
  }
  else
  if (this->bIsCard(CK_CMPPRED)) {
    if (_pCard_CmpPred(this)->bMaybeBoundsOf_index(p_index, bParamAllowedInBounds))
      return this;
    return nullptr;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index               */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_PRED::pPredFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return _pCard_OrPred(this)->pOrPredFindBound(pIndex, eBound);

    case CK_ANDPRED:
      return _pCard_AndPred(this)->pAndPredFindBound(pIndex, eBound);

    case CK_CMPPRED:
      return _pCard_CmpPred(this)->pCmpPredFindBound(pIndex, eBound);

    default:
      vTrace("pPredFindBound: unexpected eCard %lld %s\n"_Stz
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
bool Card_PRED::bPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return _pCard_OrPred(this)->bOrPredHasBounds(pIndex);

    case CK_ANDPRED:
      return _pCard_AndPred(this)->bAndPredHasBounds(pIndex);

    case CK_CMPPRED:
      return _pCard_CmpPred(this)->bCmpPredHasBounds(pIndex);

    default:
      vTrace("bPredHasBounds: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of pred and apply lambda function                      */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bPredRecursive(std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bContinue;

  eCK eCard = this->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // only call term once

  if (CK_TERM == eCard) {
    bContinue = bfApply(this,false);
    return bContinue;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // apply pred function for downwards
  bContinue = bfApply(this,false);
  if (! bContinue)
    return false;

  // descend into children
  i8 nSubCards = nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pGetChild(i_Child);
    bContinue = static_cast<Card_PRED *>(pSubCard)->bPredRecursive(bfApply);
    if (! bContinue)
      return false;
  }

  // apply pred function for upwards
  bContinue = bfApply(this,true);
  return bContinue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
