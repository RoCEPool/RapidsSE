/*============================================================================*/
/* Card_OrConj.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_OrConj.hpp"
#include "Card_CONJUNCTION.hpp"

#include "Kid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_OrConj, CK_ORCONJ)
__aNewFunction(Card_OrConj, CN_CARD)
__vDeleteFunction(Card_OrConj, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrConj::Card_OrConj(void)
: Card_CONJUNCTION(CK_ORCONJ, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OrConj::~Card_OrConj(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_OrConj::vEnsureTypesFor_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xOrConj(this);
  while (xOrConj.bEach()) {
    Card* pCard = xOrConj.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_CMPCONJ: _pCard_CmpConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_NOTCONJ: _pCard_NotConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_ORCONJ:  _pCard_OrConj(pCard)->vEnsureTypesFor_stmCol(pCI);  break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeFor_stmCol(pCI);     break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpConj* Card_OrConj::pOrPredFindBound(Index* pIndex, ePB eBound)
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
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_OrConj::bOrPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJUNCTION* pSubPred0 = (Card_CONJUNCTION*)this->pGet(0);
  if (pSubPred0->bConjHasBounds(pIndex))
    return true;

  Card_CONJUNCTION* pSubPred1 = (Card_CONJUNCTION*)this->pGet(1);
  if (pSubPred1->bConjHasBounds(pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_OrConj::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
