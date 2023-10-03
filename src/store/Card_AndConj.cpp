/*============================================================================*/
/* Card_AndConj.cpp -- And Predicate                                          */
/*============================================================================*/

#include "Card_AndConj.hpp"
#include "Card_CONJUNCTION.hpp"
#include "Card_CmpConj.hpp"

#include "Kid.hpp"
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_AndConj, CK_ANDCONJ)
__aNewFunction(Card_AndConj, CN_CARD)
__vDeleteFunction(Card_AndConj, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AndConj::Card_AndConj(void)
: Card_CONJUNCTION(CK_ANDCONJ, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AndConj::~Card_AndConj(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpConj* Card_AndConj::pAndPredFindBound(Index* pIndex, ePB eBound)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

Card_CONJUNCTION* pSubPred0 = (Card_CONJUNCTION*)this->pGet(0);
if (Card_CmpConj* pFound = pSubPred0->pPredFindBound(pIndex, eBound))
return pFound;

Card_CONJUNCTION* pSubPred1 = (Card_CONJUNCTION*)this->pGet(1);
if (Card_CmpConj* pFound = pSubPred1->pPredFindBound(pIndex, eBound))
return pFound;

return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Card_AndConj::bAndPredHasBounds(Index* pIndex)
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
void Card_AndConj::vEnsureTypesFor_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xAndPred(this);
  while (xAndPred.bEach()) {
    Card* pCard = xAndPred.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vEnsureTypesFor_stmOdn(pCI); break;
      case CK_CMPCONJ:
        _pCard_CmpConj(pCard)->vEnsureTypesFor_stmOdn(pCI); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vEnsureTypesFor_stmOdn(pCI); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vEnsureTypesFor_stmOdn(pCI);  break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeFor_stmOdn(pCI);     break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_AndConj::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_CMPCONJ:
        _pCard_CmpConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vValidate(pCI, iDepth + 1, this);  break;

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
