/*============================================================================*/
/* Card_JoinPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinPredList.hpp"

#include "CardIter.hpp"

#include "Card_BoolPred.hpp"
#include "Card_RlnConj.hpp"
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

  Kid xJoinPredList(p_deck);
  while (xJoinPredList.bEach(CK_o_FROM, CK_s_TABLE, CK_t_INDEXLIST, CK_o_FILTER)) {

    p_joinPredList->vBuildList(_pCard_Filter(xJoinPredList.pSub3()),
                               _pCard_IndexList(xJoinPredList.pSub2()));
  }

  return p_joinPredList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vTablesWithNoIndexes(Card_Deck* p_deck, String* pString)
{
  _static_

  Kid xJoinPredLisy(p_deck);
  while (xJoinPredLisy.bEach(CK_s_TABLE)) {
    Card_IndexList* p_indexList;
    if ((p_indexList = xJoinPredLisy.p_table()->pSub_indexList()) &&
        p_indexList->nChildCards())
      continue;

    if (! pString->nLength())
      pString->vSprint("Tables: %s"_Stz, xJoinPredLisy.p_table()->pSub_tableNm()->szFacet(SF_TBL_NM));
    else
      pString->vCatSprint(", %s"_Stz, xJoinPredLisy.p_table()->pSub_tableNm()->szFacet(SF_TBL_NM));
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

  //p_indexList->vDump(__func__);

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  auto bfConcat = [this, p_indexList](Card *pCard_cmpPred) {
    Card_RlnConj* p_rlnConj_tst = _pCard_RlnConj(pCard_cmpPred);

    if (! p_rlnConj_tst->bIsEqOp())
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_0 = p_rlnConj_tst->pGetTerm(0);
    Card_Term* p_term_1 = p_rlnConj_tst->pGetTerm(1);
    if ( ! (p_term_0->pSymbol()->bHasFacet(SF_TBL_COL) &&
            p_term_1->pSymbol()->bHasFacet(SF_TBL_COL)))
      return C_CONTINUE_RECURSION;

    if (!p_indexList->bHasEquivalentBound(p_rlnConj_tst)) {
      //p_rlnConj_tst->vDump(__func__,"NO EquivalentBound"_Stz);
      return C_CONTINUE_RECURSION;
    }

    Card_RlnConj* p_rlnConj_new = p_rlnConj_tst->pClone();
    this->vAppendChild(p_rlnConj_new);

    return C_CONTINUE_RECURSION;
  };

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  Card_BoolPred* p_boolPred = p_filter->pSub_boolPred();
  p_boolPred->bLambdaWalk(CK_RLNCONJ, bfConcat);
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
