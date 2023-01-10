/*============================================================================*/
/* Card_ELIST.cpp -- Abstract Expression List                                 */
/*============================================================================*/

#include "Card_ELIST.hpp"
#include "Card_As.hpp"
#include "Card_StmCol.hpp"
#include "Card_OrdElmOdn.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ELIST::Card_ELIST(eCK eCard)
: Card(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ELIST::~Card_ELIST(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vBump_keyOdn_In_rpnCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while ((pCI->G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    pCI->G.p_rpnCalc = pCI->G.p_expr->pSub_rpnCalc();

    if (! pCI->G.p_rpnCalc)
      continue;

    CardIter xCI2(pCI->G.p_rpnCalc);
    while ((pCI->G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = pCI->G.p_term->szValue();

      if (Card::bIsKeyOrdinal(szTerm)) {
        pCI->G.p_term->vBump_KeyOrdinal();
        pCI->G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vBump_stmOdn_In_rpnCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while ((pCI->G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    pCI->G.p_rpnCalc =pCI->G.p_expr->pSub_rpnCalc();

    if (! pCI->G.p_rpnCalc)
      continue;

    CardIter xCI2(pCI->G.p_rpnCalc);
    while ((pCI->G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = pCI->G.p_term->szValue();

      if (Card::bIsStreamColOrdinal(szTerm)) {
        pCI->G.p_term->vBump_StreamColOrdinal();
        pCI->G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_ELIST::iRpnCalc_Has_stmOrdOdn(Card_OrdElmOdn* p_stmOrdOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szStrOrdOdn = p_stmOrdOdn->szValue();

  CardIter xCI_elist(this);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {

    Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
    if (! p_rpnCalc)
      continue;

    if (p_rpnCalc->nChildCards() > 1)
      continue;

    CardIter xCI_rpnCalc(p_rpnCalc);
    while (Card_Term* p_term = _pCard_Term(xCI_rpnCalc.pEachChild())) {
      if (p_term->bIsValue(szStrOrdOdn))
        return xCI_elist.i_Child();
    }
  }

  return C_UNKNOWN;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
