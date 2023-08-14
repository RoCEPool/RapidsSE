/*============================================================================*/
/* Card_NotPred.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_NotPred.hpp"
#include "Card_PRED.hpp"

#include "Kid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_NotPred, CK_NOTPRED)
__aNewFunction(Card_NotPred, CN_CARD)
__vDeleteFunction(Card_NotPred, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_NotPred::Card_NotPred(void)
: Card_PRED(CK_NOTPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_NotPred::~Card_NotPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_NotPred::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
