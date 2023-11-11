/*============================================================================*/
/* Card_OrConj.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_OrConj.hpp"
#include "Card_CONJ.hpp"

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
: Card_CONJ(CK_ORCONJ, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  pSymbol()->vAppendFacet(SF_BOL_OP,"or"_Stz);

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
void Card_OrConj::vEnsureTypeId_expCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xOrConj(this);
  while (xOrConj.bEach()) {
    Card* pCard = xOrConj.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ:
        _pCard_AndConj(pCard)->vEnsureTypeId_expCol(pCI); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vEnsureTypeId_expCol(pCI); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vEnsureTypeId_expCol(pCI);  break;
      case CK_RLNCONJ:
        _pCard_RlnConj(pCard)->vEnsureTypeId_expCol(pCI); break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeId_expCol(pCI);     break;

      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_OrConj::pOrPredFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJ* pSubConj0 = (Card_CONJ*)this->pGet(0);
  if (Card_RlnConj* pFound = pSubConj0->pPredFindBound(pIndex, eBound))
    return pFound;

  Card_CONJ* pSubConj1 = (Card_CONJ*)this->pGet(1);
  if (Card_RlnConj* pFound = pSubConj1->pPredFindBound(pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
//bool Card_OrConj::bOrPredHasBounds(Index* pIndex)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  Card_CONJ* pSubConj0 = (Card_CONJ*)this->pGet(0);
//  if (pSubConj0->bConjHasBounds(pIndex))
//    return true;
//
//  Card_CONJ* pSubConj1 = (Card_CONJ*)this->pGet(1);
//  if (pSubConj1->bConjHasBounds(pIndex))
//    return true;
//
//  return false;
//}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_OrConj::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);

  Card::vValidate_hasChildren();
  Card::eValidate_mustSome(CK_ANDCONJ,CK_NOTCONJ,CK_ORCONJ,CK_RLNCONJ,CK_NONE);

  (void)Card::eValidate_parentEither(pCI,CK_BOOLPRED,
                                     CK_ANDCONJ,CK_NOTCONJ,CK_ORCONJ,
                                     CK_NONE);

  Kid xRpnCalc(this);
  while (xRpnCalc.bEach()) {
    Card* pCard = xRpnCalc.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vValidate(pCI, iDepth + 1, this); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vValidate(pCI, iDepth + 1, this);  break;
      case CK_RLNCONJ:
        _pCard_RlnConj(pCard)->vValidate(pCI, iDepth + 1, this); break;

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
