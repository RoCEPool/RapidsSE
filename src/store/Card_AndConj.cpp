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

  pSymbol()->vApplyFacet(SF_BOL_OP,"and"_Stz);
  
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
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_AndConj::vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Cast xCast_last;
  xCast_last.m_xTypeId = TypeId();
  xCast_last.m_nTypeLength = 0;
  xCast_last.m_nTypeScale = 0;
  RASSERT(! xCast_last.m_xTypeId.bIsValid());

  Kid xAndConj(this);
  while (xAndConj.bEach()) {
    Card* pCard = xAndConj.pChild();
    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_ANDCONJ:
        _pCard_AndConj(pCard)->vApplyCast(pCI, /*??*/p_Dic_expr); break;
      case CK_NOTCONJ:
        _pCard_NotConj(pCard)->vApplyCast(pCI, /*??*/p_Dic_expr); break;
      case CK_ORCONJ:
        _pCard_OrConj(pCard)->vApplyCast(pCI, /*??*/p_Dic_expr);  break;
      case CK_RLNCONJ:
        _pCard_RlnConj(pCard)->vApplyCast(pCI, /*??*/p_Dic_expr); break;

      case CK_TERM:
        _pCard_Term(pCard)->vApplyCast(pCI, /*??*/p_Dic_expr, xCast_last);  break;

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
