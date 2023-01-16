/*============================================================================*/
/* Card_Where.cpp -- Where                                                    */
/*============================================================================*/

#include "Card_Where.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Where,CK_WHERE)
__aNewFunction(Card_Where,CN_CARD)
__vDeleteFunction(Card_Where,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Where::Card_Where(void)
: Card(CK_WHERE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Where::~Card_Where(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vAdd_where(CardIter* pCI, Card* p_after)
{
  _static_

  pCI->G_vNewMember(CK_WHERE);

  pCI->G.pDeck->vInsertAfter(pCI->G.p_where,p_after);

  pCI->G_vAppendNewChild(pCI->G.p_where,CK_RPNCALC);
}

/*----------------------------------------------------------------------------*/
/* move all <absJoin>/<on>/<rpnCalc> to <where>                               */
/*----------------------------------------------------------------------------*/
void Card_Where::vMove_on_To_where(CardIter* pCI)
{
  _static_

  // find the <where> (and /<rpnCalc> in the pipeline
  // n.b. there might not be a <where> yet

  pCI->vRestart();
  if (pCI->G_bEachMain(CK_WHERE))
    pCI->G.p_rpnCalc = pCI->G.p_where->pSub_rpnCalc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each <absJoin>/<on>/<rpnCalc>
  //   move/append to <where>/<rpnCalc>

  pCI->vRestart();
  while (pCI->G_bEachMainWithSub_reverse(CK_ABSJOIN,CK_ON)) {

    Card_RpnCalc *p_rpnCalc_on = pCI->G.p_on->pSub_rpnCalc();
    if (! p_rpnCalc_on->nChildCards())
      continue;
    Card_Term *p_term_0 = p_rpnCalc_on->pGetTerm(0);
    if (p_term_0->bSame("CROSS"_Stz))
      continue;

    if (! pCI->G.p_where) {
      // create a new <where> if non exists
      pCI->G_vNewMember(CK_WHERE);
      pCI->G.pDeck->vInsertAfter(pCI->G.p_where,pCI->G.p_absJoin);

      // create an initial empty /<rpnCalc>
      pCI->G_vAppendNewChild(pCI->G.p_where,CK_RPNCALC);
    }

    // append the <absJoin>/<on>/<rpnCalc> to the <where>/<rpnCalc>
    pCI->G.p_rpnCalc->vAppendRpnCalc(p_rpnCalc_on);

    // remove and delete <absJoin>/<on>
    pCI->G.p_absJoin->vRemoveChild(pCI->G.p_on);
    DELETE_CARD(pCI->G.p_on);
  }
}

/*----------------------------------------------------------------------------*/
/* move <where> to <table keyExists>/<constraint>                             */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWhereToTableAsConstraint(CardIter* pCI)
{
  _static_

  pCI->G.p_boolCalc = pCI->G.p_where->pSub_boolCalc();
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));
  pCI->G.p_PRED->vMark1(MARK_AT);

  pCI->G.p_subSet = pCI->G.p_boolCalc->pDupSubCards();
  RASSERT(1 == pCI->G.p_subSet->nChildCards());
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_subSet->pPop());
  DELETE_CARD(pCI->G.p_subSet);

  if ((pCI->G.p_constraint = pCI->G.p_table->pSub_constraint())) {
    pCI->G.p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
    pCI->G.p_boolCalc->vPushPred(pCI->G.p_PRED);
  }
  else {
    pCI->G_vAppendNewChild(pCI->G.p_table,CK_CONSTRAINT);
    pCI->G_vAppendNewChild(pCI->G.p_constraint,CK_BOOLCALC);
    pCI->G_vAppendChild(CK_BOOLCALC,pCI->G.p_PRED);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
