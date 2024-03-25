/*============================================================================*/
/* Card_RpnCalc.cpp -- RPN Calculation Sequence                               */
/*============================================================================*/

#include "Card_RpnCalc.hpp"
#include "Card_Term.hpp"
#include "Card_BoolPred.hpp"
#include "CardIter.hpp"
#include "Card_RpnExpr.hpp"
#include "Card_AndConj.hpp"
#include "Card_OrConj.hpp"
#include "Card_RlnConj.hpp"
#include "Card_NotConj.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_RpnCalc, CK_RPNCALC)
__aNewFunction(Card_RpnCalc, CN_CARD)
__vDeleteFunction(Card_RpnCalc, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::Card_RpnCalc(void)
: Card_TERMLIST(CK_RPNCALC, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::Card_RpnCalc(Symbol* pSymbol)
: Card_TERMLIST(CK_RPNCALC, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term = (new Card_Term) ->SYMPPY(pSymbol);
  vAppendChild(p_term);

  vApplyCast_p(p_term);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::~Card_RpnCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sType_short = szCT_char[eCTVal()];

  pRibbon->vSprintSnippet(false,"%c%c%s%lld]%s %s%lld"_Stz,
                          cMark1(),
                          cMark2(),
                          Stz::szDots(iDepth - 1),
                          m_i_Child,
                          szCardNm(),
                          sType_short.szStz(),
                          m_xSymbol.xCast().m_nTypeLength);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  (void)Card::eValidate_parentEither(pCI,
                                     CK_EXPR,
                                     CK_NONE);

  vValidate_mustHave(CK_TERM, CK_NONE);

  Card::vValidate_hasType();
  Card::vValidate_hasChildren();

  if (xCast().m_xTypeId != pCI->pAncestor(CK_EXPR)->xCast().m_xTypeId) {
    pCI->PRINTBOARD_dbg();
    vDump(__func__);
    NEVER_REACH;
  }

  vValidate_mustHave(CK_TERM, CK_NONE);

  (void)Card::eValidate_parentEither(pCI,CK_EXPR,CK_NONE);

  if (pCI->bAncestor(CK_s_INJECTABLE)) {
    if (!m_xSymbol.bValidType()) {
      vDump(__func__);
      vValidationFailed("Type Missing : %p"_Stz,this);
    }
  }

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
