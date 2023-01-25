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
/* compute the load of this join permutation                                   */
/*----------------------------------------------------------------------------*/
void Card_JoinPermSeq::vCalculateLoad(Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  char cTblSyn_l = ' ';

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {

    if (xCI.G.p_MainCard->bIsCard(CK_JOINHEAD)) {
      cTblSyn_l = xCI.G.p_joinHead->cValue();
      continue;
    }

    if (! xCI.G.p_MainCard->bIsCard(CK_JOINNEXT))
      continue;

    char cTblSyn_r = xCI.G.p_joinNext->cValue();

    Card_LoadJoin*    p_loadJoin    = new Card_LoadJoin;
    Card_Cast*        p_cast        = new Card_Cast;
    Card_LoadNetwork* p_loadNetwork = new Card_LoadNetwork;
    Card_LoadUnicast* p_loadUnicast = new Card_LoadUnicast;

    Card_TablePtn* p_tablePtn_l = p_tableList->pGetTablePtn(cTblSyn_l);
    Card_TablePtn* p_tablePtn_r = p_tableList->pGetTablePtn(cTblSyn_r);
    Card_CmpPred*  p_cmpPred    = xCI.G.p_joinNext->pSub_cmpPred();

    eXK eCast = p_cmpPred->eCastKind(p_tablePtn_l,p_tablePtn_r);
    p_cast->vValue(aXK[eCast]);
    p_loadJoin->vAppend(p_cast);

    p_loadNetwork->vValue((i8)0);
    p_loadUnicast->vValue((i8)0);
    i8 iLoadJoin = p_loadNetwork->iValue() + p_loadUnicast->iValue();
    p_loadJoin->vValue((i8)iLoadJoin);

    p_loadJoin->vAppend(p_loadNetwork);
    p_loadJoin->vAppend(p_loadUnicast);
    xCI.G.p_MainCard->vAppend(p_loadJoin);

    cTblSyn_l = cTblSyn_r;
  }
}

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
    char cTblSyn_ith = p_table->cValue();

    if (i_Item)
      vTrace(", "_Stz,i_Table);
    vTrace("%c"_Stz, cTblSyn_ith);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (0 == nChildCards()) {
      //+ table is head/first of join sequence
      Card_JoinHead *p_joinHead = new Card_JoinHead;
      p_joinHead->vValue(cTblSyn_ith);
      vAppend(p_joinHead);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // table is next/subsequent of join sequence

    Card_JoinNext *p_joinNext = new Card_JoinNext;
    p_joinNext->vValue(cTblSyn_ith);
    vAppend(p_joinNext);


    bool bSupported = false;

    // test each join predicate which uses the ith table to see if a join
    // can be supported at this point in the sequence
    CardIter xCI(p_joinPredList);
    while (xCI.G_bEachMain(CK_CMPPRED)) {
      if (! xCI.G.p_cmpPred->bUsesTable(cTblSyn_ith))
        continue;

      // have a join cmpPred which uses the ith table

      // get the other table in the cmpPred
      char cTblSyn_other = xCI.G.p_cmpPred->cOtherTable(cTblSyn_ith);


      if (bHasKnownJoin(cTblSyn_other)) {
        bSupported = true;
        p_joinNext->vAppend(xCI.G.p_cmpPred->pDupCardAndSubCards());
        break;
      }
    }

    if (bSupported)
      //+ join of ith table is possible at this point
      continue;

    // join of ith table is not possible at this point
    p_joinNext->vAppend(new Card_Note("**DOESN'T FOLLOW**"_Stz));
    Y_vSet(P_Invalid);
    break;
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* does this join sequence already have a join with given table at this point?*/
/*----------------------------------------------------------------------------*/
bool Card_JoinPermSeq::bHasKnownJoin(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    ASSERT(xCI.G.p_MainCard->bIsCard(CK_JOINHEAD) || xCI.G.p_MainCard->bIsCard(CK_JOINNEXT))
    if (xCI.G.p_MainCard->bIsValue(cTblSyn))
      return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
