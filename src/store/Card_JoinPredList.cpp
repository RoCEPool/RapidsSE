/*============================================================================*/
/* Card_JoinPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinPredList.hpp"

#include "CardIter.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpConj.hpp"
#include "Card_Deck.hpp"
#include "Card_Filter.hpp"
#include "Card_Term.hpp"
#include "Card_IndexList.hpp"


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
: Card_symbol(CK_JOINPREDLIST, C_UNKNOWN)
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

  Kid xKid(p_deck);
  while (xKid.bEach(CK_FROM,CK_TABLE,CK_INDEXLIST,CK_FILTER)) {

    p_joinPredList->vBuildList(_pCard_Filter(xKid.pSub3()),
                               _pCard_IndexList(xKid.pSub2()));
  }

  return p_joinPredList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vTablesWithNoIndexes(Card_Deck* p_deck, String* pString)
{
  _static_

  Kid xKid(p_deck);
  while (xKid.bEach(CK_TABLE)) {
    Card_IndexList* p_indexList;
    if ( (p_indexList = xKid.p_table()->pSub_indexList()) &&
        p_indexList->nChildCards())
      continue;

    if (! pString->nLength())
      pString->vSprint("Tables: %s"_Stz,xKid.p_table()->pSub_tableNm()->szSymbol(SF_NONE));
    else
      pString->vCatSprint(", %s"_Stz,xKid.p_table()->pSub_tableNm()->szSymbol(SF_NONE));
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
    Card_CmpConj* p_cmpConj_tst = _pCard_CmpConj(pCard_cmpPred);

    if (! p_cmpConj_tst->bIsEqOp())
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_0 = p_cmpConj_tst->pGetTerm(0);
    Card_Term* p_term_1 = p_cmpConj_tst->pGetTerm(1);
    if ( ! (p_term_0->bIsTblCol() && p_term_1->bIsTblCol()))
      return C_CONTINUE_RECURSION;

    if (!p_indexList->bHasEquivalentBound(p_cmpConj_tst))
      return C_CONTINUE_RECURSION;

    Card_CmpConj* p_cmpConj_new = p_cmpConj_tst->pClone();
    this->vAppendChild(p_cmpConj_new);

    return C_CONTINUE_RECURSION;
  };

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  p_boolCalc->bApplyRecursive(CK_CMPCONJ, bfConcat);
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
