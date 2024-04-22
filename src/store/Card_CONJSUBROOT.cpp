/*============================================================================*/
/* Card_CONJSUBROOT.cpp -- Abstract Conjunction Sub-Root                      */
/*============================================================================*/

#include "Card_CONJSUBROOT.hpp"
#include "Card_Term.hpp"
#include "Card_OrConj.hpp"
#include "Card_AndConj.hpp"
#include "Card_RlnConj.hpp"
#include "CardIter.hpp"
#include "Card_Table.hpp"
#include "Card_Deck.hpp"
#include "Scope.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CONJSUBROOT::Card_CONJSUBROOT(eCK eCard, i8 iFixedDepth)
: Card_symbol(eCard, iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CONJSUBROOT::~Card_CONJSUBROOT(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Card_CONJSUBROOT::vOntoRpnExpr(String* psRpnExpr, Card_CONJSUBROOT* p_conj)
{
  _static_;

  UNUSED(psRpnExpr);

  Kid xConj(p_conj);

  eCK eConj = p_conj->eCard();
  switch (eConj) {

    case CK_ANDCONJ:
      RASSERT(xConj.bEach());
      vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
      RASSERT(xConj.bEach());
      vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
      psRpnExpr->vCatSprint(" and"_Stz);

      while (xConj.bEach()) {
        vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
        psRpnExpr->vCatSprint(" and"_Stz);
      }
      break;

    case CK_NOTCONJ:
      RASSERT(xConj.bEach());
      vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
      psRpnExpr->vCatSprint(" not"_Stz);
      break;

    case CK_ORCONJ:
      RASSERT(xConj.bEach());
      vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
      RASSERT(xConj.bEach());
      vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
      psRpnExpr->vCatSprint(" or"_Stz);

      while (xConj.bEach()) {
        vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
        psRpnExpr->vCatSprint(" or"_Stz);
      }
      break;

    case CK_RLNCONJ:
      while (xConj.bEach())
        vOntoRpnExpr(psRpnExpr,(Card_CONJSUBROOT*)xConj.pChild());
      psRpnExpr->vCatSprint(" %s"_Stz, p_conj->pSymbol()->szFacet0());
      break;

    case CK_TERM:
      psRpnExpr->vCatSprint(" %s"_Stz, p_conj->pSymbol()->szFacet_rpnTerm());
      break;

    default:
      NEVER_REACH
  }

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get child andConj                                                          */
/*----------------------------------------------------------------------------*/
Card_AndConj* Card_CONJSUBROOT::pGet_andConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_AndConj* p_andConj = _pCard_AndConj(pGetChild(i_Child));

  return p_andConj;
}

/*----------------------------------------------------------------------------*/
/* get child notConj                                                          */
/*----------------------------------------------------------------------------*/
Card_NotConj* Card_CONJSUBROOT::pGet_notConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_NotConj* p_notConj = _pCard_NotConj(pGetChild(i_Child));

  return p_notConj;
}

/*----------------------------------------------------------------------------*/
/* get child orConj                                                           */
/*----------------------------------------------------------------------------*/
Card_OrConj* Card_CONJSUBROOT::pGet_orConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_OrConj* p_orConj = _pCard_OrConj(pGetChild(i_Child));

  return p_orConj;
}

/*----------------------------------------------------------------------------*/
/* get child rlnConj                                                          */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_CONJSUBROOT::pGet_rlnConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* p_rlnConj = _pCard_RlnConj(pGetChild(i_Child));

  return p_rlnConj;
}

/*----------------------------------------------------------------------------*/
/* append part                                                                */
/*----------------------------------------------------------------------------*/
void Card_CONJSUBROOT::vOntoRpnExpr(String* psRpnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xCONJSUBROOT(this); xCONJSUBROOT.bEach(); /**/) {
    Card *pChild = xCONJSUBROOT.pChild();

    eCK eChild = pChild->eCard();
    switch (eChild) {
      case CK_ANDCONJ:
        _pCard_AndConj(pChild)->vOntoRpnExpr(psRpnExpr);
        psRpnExpr->vCatSprint(" and"_Stz);
        break;
      case CK_NOTCONJ:
        _pCard_NotConj(pChild)->vOntoRpnExpr(psRpnExpr);
        psRpnExpr->vCatSprint(" not"_Stz);
        break;
      case CK_ORCONJ:
        _pCard_OrConj(pChild)->vOntoRpnExpr(psRpnExpr);
        psRpnExpr->vCatSprint(" or"_Stz);
        break;
      case CK_RLNCONJ:
        _pCard_RlnConj(pChild)->vOntoRpnExpr(psRpnExpr);
        psRpnExpr->vCatSprint(" %s"_Stz, pChild->pSymbol()->szFacet0());
        break;
      case CK_TERM:
        if (pChild->pSymbol()->bHasFacet(SF_EXP_COL) ||
            pChild->pSymbol()->bHasFacet(SF_TBL_COL)) {

          // TODO: this rule should be in validate?
          if (!pChild->pSymbol()->bValidType()) {
            //p_part->vDump();
            //pChild->vDump();
            RASSERT(pChild->pSymbol()->bValidType());
          }
        }
        psRpnExpr->vCatSprint(" %s"_Stz, pChild->pSymbol()->szFacet_rpnTerm());
        break;
      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/* pop child andConj                                                          */
/*----------------------------------------------------------------------------*/
Card_AndConj* Card_CONJSUBROOT::pPop_andConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_AndConj* p_andConj = _pCard_AndConj(pPopChild());

  return p_andConj;
}

/*----------------------------------------------------------------------------*/
/* pop child notConj                                                          */
/*----------------------------------------------------------------------------*/
Card_NotConj* Card_CONJSUBROOT::pPop_notConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_NotConj* p_notConj = _pCard_NotConj(pPopChild());

  return p_notConj;
}

/*----------------------------------------------------------------------------*/
/* pop child orConj                                                           */
/*----------------------------------------------------------------------------*/
Card_OrConj* Card_CONJSUBROOT::pPop_orConj (void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_OrConj* p_orConj = _pCard_OrConj(pPopChild());

  return p_orConj;
}

/*----------------------------------------------------------------------------*/
/* pop child rlnConj                                                          */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_CONJSUBROOT::pPop_rlnConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* p_rlnConj = _pCard_RlnConj(pPopChild());

  return p_rlnConj;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
