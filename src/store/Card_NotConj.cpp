/*============================================================================*/
/* Card_NotConj.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_NotConj.hpp"
#include "Card_CONJUNCTION.hpp"

#include "Kid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_NotConj, CK_NOTCONJ)
__aNewFunction(Card_NotConj, CN_CARD)
__vDeleteFunction(Card_NotConj, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_NotConj::Card_NotConj(void)
: Card_CONJUNCTION(CK_NOTCONJ, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_NotConj::~Card_NotConj(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_NotConj::vEnsureTypesFor_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xNotPred(this);
  while (xNotPred.bEach()) {
    Card* pCard = xNotPred.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_CMPCONJ:
        _pCard_CmpConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vEnsureTypesFor_stmCol(pCI);  break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeFor_stmCol(pCI);     break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_NotConj::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
