/*============================================================================*/
/* Card_AndConj.cpp -- And Predicate                                          */
/*============================================================================*/

#include "Card_AndConj.hpp"
#include "Card_CONJ.hpp"
#include "Card_RlnConj.hpp"

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
: Card_CONJ(CK_ANDCONJ, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  pSymbol()->vAppendFacet(SF_BOL_OP,"and"_Stz);
  
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
Card_RlnConj* Card_AndConj::pAndConjFindBound(Index* pIndex, ePB eBound)
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
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Card_AndConj::bAndPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJ* pSubConj0 = (Card_CONJ*)this->pGet(0);
  if (pSubConj0->bConjHasBounds(pIndex))
    return true;

  Card_CONJ* pSubConj1 = (Card_CONJ*)this->pGet(1);
  if (pSubConj1->bConjHasBounds(pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_AndConj::vEnsureTypesFor_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xAndConj(this);
  while (xAndConj.bEach()) {
    Card* pCard = xAndConj.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ: _pCard_AndConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_NOTCONJ: _pCard_NotConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;
      case CK_ORCONJ:  _pCard_OrConj(pCard)->vEnsureTypesFor_stmCol(pCI);  break;
      case CK_RLNCONJ: _pCard_RlnConj(pCard)->vEnsureTypesFor_stmCol(pCI); break;

      case CK_TERM:    _pCard_Term(pCard)->vEnsureTypeFor_stmCol(pCI);     break;

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
