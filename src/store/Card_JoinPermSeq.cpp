/*============================================================================*/
/* Card_JoinPermSeq.cpp -- Join Permutation Sequence                          */
/*============================================================================*/

#include "Card_JoinPermSeq.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CmpPred.hpp"
#include "Card_JoinPermAll.hpp"
#include "Card_JoinPredList.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinPermSeq, CK_JOINPERMSEQ)
__aNewFunction(Card_JoinPermSeq, CN_CARD)
__vDeleteFunction(Card_JoinPermSeq, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPermSeq::Card_JoinPermSeq(void)
: Card(CK_JOINPERMSEQ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPermSeq::~Card_JoinPermSeq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute one permuted join sequence                                         */
/*----------------------------------------------------------------------------*/
void Card_JoinPermSeq::vOnePermutedJoin(Card_TableList*    p_tableList,
                                        Card_JoinPredList* p_joinPredList,
                                        PermGen*           pPermGen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nTables = p_tableList->nChildCards();

  vTrace("%lld :"_Stz,pPermGen->nPermutations());
  for (i8 i_Item = 0; i_Item < nTables; i_Item++) {

    // get the ith table of this permutation
    i8 i_Table = pPermGen->iItem(i_Item);
    Card_Table* p_table = _pCard_Table(p_tableList->pGetChild(i_Table));
    char cTblSyn = p_table->cValue();

    if (i_Item)
      vTrace(", "_Stz,i_Table);
    vTrace("%c"_Stz,cTblSyn);

    if (0 == nChildCards()) {
      Card_JoinHead *p_joinHead = new Card_JoinHead;
      p_joinHead->vValue(cTblSyn);
      vAppend(p_joinHead);
      continue;
    }

    Card_JoinNext *p_joinNext = new Card_JoinNext;

    // get the join pred for this table
    Card_CmpPred* p_cmpPred_join = p_joinPredList->pFindCmpPred(cTblSyn);
    Card_CmpPred* p_cmpPred_perm = _pCard_CmpPred(p_cmpPred_join->pDupCardAndSubCards());
    p_joinNext->vAppend(p_cmpPred_perm);

    char cTblSyn_prior = p_cmpPred_join->cOtherTable(cTblSyn);
    if (! bSupportedJoin(cTblSyn_prior)) {
      p_joinNext->vAppend(new Card_Note("**DOESN'T FOLLOW**"_Stz));
      Y_vSet(P_Invalid);
      break;
    }

  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* does this permutation support join to given table at this point?           */
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bSupportedJoin(char cTblSyn_prior)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! nChildCards())
    return true;

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_JOINHEAD)) {
    if (xCI.G.p_joinHead->cValue() == cTblSyn_prior)
      return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
