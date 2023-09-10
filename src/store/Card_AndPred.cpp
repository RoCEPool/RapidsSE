/*============================================================================*/
/* Card_AndPred.cpp -- And Predicate                                          */
/*============================================================================*/

#include "Card_AndPred.hpp"
#include "Card_CONJUNCTION.hpp"
#include "Card_CmpPred.hpp"

#include "Kid.hpp"
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_AndPred, CK_ANDPRED)
__aNewFunction(Card_AndPred, CN_CARD)
__vDeleteFunction(Card_AndPred, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AndPred::Card_AndPred(void)
: Card_CONJUNCTION(CK_ANDPRED, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AndPred::~Card_AndPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_AndPred::pAndPredFindBound(Index* pIndex, ePB eBound)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

Card_CONJUNCTION* pSubPred0 = (Card_CONJUNCTION*)this->pGet(0);
if (Card_CmpPred* pFound = pSubPred0->pPredFindBound(pIndex, eBound))
return pFound;

Card_CONJUNCTION* pSubPred1 = (Card_CONJUNCTION*)this->pGet(1);
if (Card_CmpPred* pFound = pSubPred1->pPredFindBound(pIndex, eBound))
return pFound;

return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Card_AndPred::bAndPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJUNCTION* pSubPred0 = (Card_CONJUNCTION*)this->pGet(0);
  if (pSubPred0->bPredHasBounds(pIndex))
    return true;

  Card_CONJUNCTION* pSubPred1 = (Card_CONJUNCTION*)this->pGet(1);
  if (pSubPred1->bPredHasBounds(pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_AndPred::vEnsureTypesFor_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xAndPred(this);
  while (xAndPred.bEach()) {
    Card* pCard = xAndPred.pChild();
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
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_AndPred::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
        vValidationFailed("<<replace>> %p"_Stz,this);
    }
  }

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
