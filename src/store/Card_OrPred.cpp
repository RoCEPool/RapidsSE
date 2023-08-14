/*============================================================================*/
/* Card_OrPred.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_OrPred.hpp"
#include "Card_PRED.hpp"

#include "Kid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_OrPred, CK_ORPRED)
__aNewFunction(Card_OrPred, CN_CARD)
__vDeleteFunction(Card_OrPred, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrPred::Card_OrPred(void)
: Card_PRED(CK_ORPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OrPred::~Card_OrPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_OrPred::vEnsureTypesFor_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xOrPred(this);
  while (xOrPred.bEach()) {
    Card* pCard = xOrPred.pChild();
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
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_OrPred::pOrPredFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)this->pGet(0);
  if (Card_CmpPred* pFound = pSubPred0->pPredFindBound(pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)this->pGet(1);
  if (Card_CmpPred* pFound = pSubPred1->pPredFindBound(pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_OrPred::bOrPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)this->pGet(0);
  if (pSubPred0->bPredHasBounds(pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)this->pGet(1);
  if (pSubPred1->bPredHasBounds(pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_OrPred::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
