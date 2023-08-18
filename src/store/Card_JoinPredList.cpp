/*============================================================================*/
/* Card_JoinPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinPredList.hpp"

#include "CardIter.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Deck.hpp"
#include "Card_Filter.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPredList,CK_JOINPREDLIST)
__aNewFunction(Card_JoinPredList,CN_CARD)
__vDeleteFunction(Card_JoinPredList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::Card_JoinPredList(void)
: Card_symbol(CK_JOINPREDLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::~Card_JoinPredList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_JoinPredList* Card_JoinPredList::pExtractList(Card_Deck* p_deck)
{
  _static_

  Card_JoinPredList* p_joinPredList = new Card_JoinPredList;

  CardIter xCI(p_deck);
  while (xCI.G_bEachMainWithSub(CK_FROM,CK_TABLE)) {
    if (! (xCI.G.p_indexList = xCI.G.p_MainCard->pSub_indexList()) )
      continue;
    if (! (xCI.G.p_filter = xCI.G.p_MainCard->pSub_filter()) )
      continue;

    p_joinPredList->vBuildList(xCI.G.p_filter,xCI.G.p_indexList);
  }

  return p_joinPredList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vTablesWithNoIndexes(Card_Deck* p_deck, String* pString)
{
  _static_

  CardIter xCI(p_deck);
  while (xCI.G_bEachMain(CK_TABLE)) {
    if ((xCI.G.p_indexList = xCI.G.p_table->pSub_indexList()) &&
        xCI.G.p_indexList->nChildCards())
      continue;

    if (! pString->nLength())
      pString->vSprint("Tables: %s"_Stz,xCI.G.p_table->pSub_tableNm()->szSymbol(SF_NONE));
    else
      pString->vCatSprint(", %s"_Stz,xCI.G.p_table->pSub_tableNm()->szSymbol(SF_NONE));
  }

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* build the join predicate list from given filter & indexList                */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vBuildList(Card_Filter*    p_filter,
                                   Card_IndexList* p_indexList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  auto bfConcat = [this, p_indexList](Card *pCard_cmpPred) {
    Card_CmpPred* p_cmpPred_tst = _pCard_CmpPred(pCard_cmpPred);

    if (! p_cmpPred_tst->bIsEqOp())
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_0 = p_cmpPred_tst->pGetTerm(0);
    Card_Term* p_term_1 = p_cmpPred_tst->pGetTerm(1);
    if ( ! (p_term_0->bIsColOdn() && p_term_1->bIsColOdn()))
      return C_CONTINUE_RECURSION;

    if (!p_indexList->bHasEquivalentBound(p_cmpPred_tst))
      return C_CONTINUE_RECURSION;

    Card_CmpPred* p_cmpPred_new = p_cmpPred_tst->pClone();
    this->vAppend(p_cmpPred_new);

    return C_CONTINUE_RECURSION;
  };

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  p_boolCalc->bApplyRecursive(CK_CMPPRED, bfConcat);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
