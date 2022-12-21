/*============================================================================*/
/* Card_JoinOrdering.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinOrdering.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Deck.hpp"
#include "Card_JoinOrder.hpp"
#include "Card_Permutation.hpp"
#include "Card_PtndJoin.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinOrdering, CK_JOINORDERING)
__aNewFunction(Card_JoinOrdering, CN_CARD)
__vDeleteFunction(Card_JoinOrdering, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinOrdering::Card_JoinOrdering(void)
: Card(CK_JOINORDERING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinOrdering::~Card_JoinOrdering(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vEstimateJoinedBuff(Card_Deck* pDeck,
                                         Card_TableList* p_tableList)
{
  _static_



  CardIter xCI(pDeck);

//  while (G.p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN))) {
  while (Card* pMainCard = xCI.pEachChild()) {
    eCK eCard = pMainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      p_tableList->Y_vCutRecursive(P_TableUsed);
      continue;
    }

    if (CK_PTNDJOIN != eCard)
      continue;

    Card_PtndJoin* p_ptndJoin = _pCard_PtndJoin(pMainCard);
    p_ptndJoin->vEstimateJoinBuff(p_tableList);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vEstimateJoinOrder(Card_Deck* pDeck)
{
  _static_

  i8 iLowestNetwork = 999999999;
  Card_Permutation* p_permutation_low = nullptr;

  CardIter xCI(pDeck);

  while (Card_Permutation* p_permutation =
             _pCard_Permutation(xCI.pEachChild(CK_PERMUTATION))) {

    i8 iNetwork = p_permutation->Y_iGet(P_Network);
    if (iNetwork >= iLowestNetwork)
      continue;
    p_permutation_low = p_permutation;
    iLowestNetwork = iNetwork;
  }

  Card_JoinOrdering* p_joinOrderSeq = new Card_JoinOrdering;
  pDeck->vAppendMain(p_joinOrderSeq);
  p_joinOrderSeq->Y_vSet(P_Network, iLowestNetwork);

  xCI.vRestart();
  xCI.vSet(p_permutation_low);
  while (Card_PtndJoin* p_ptndJoin = _pCard_PtndJoin(xCI.pEachChild(CK_PTNDJOIN))) {
    p_joinOrderSeq->vConcat(p_ptndJoin);
  }

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vEstimateNetwork(Card_Deck* pDeck)
{
  _static_

  i8 nNetwork = 0;
  i8 nSentOut = 0;
  Card_Permutation* p_permutation = nullptr;
  Card_PtndJoin* p_ptndJoin = nullptr;


  CardIter xCI(pDeck);

  while (Card* pMainCard = xCI.pEachChild()) {

    eCK eCard = pMainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      if (p_permutation)
        p_permutation->Y_vSet(P_Network, nNetwork);
      p_permutation = _pCard_Permutation(pMainCard);

      p_ptndJoin = nullptr;
      nNetwork = 0;
      nSentOut = 0;
    }
    else
    if (CK_PTNDJOIN == eCard) {
      if (p_ptndJoin) {
        nSentOut += p_ptndJoin->Y_iGet(P_JoinedBuff);
        nNetwork += nSentOut;
        p_ptndJoin->Y_vSet(P_SentOut, nSentOut);
      }
      p_ptndJoin = _pCard_PtndJoin(pMainCard);
    }
  }

  if (p_permutation)
    p_permutation->Y_vSet(P_Network, nNetwork);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vConcat(Card_PtndJoin* p_PtndJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eChild;

  for (i8 i_Child = 0; i_Child < p_PtndJoin->nChildCards(); i_Child++) {
    Card_CmpPred* p_predCmp = _pCard_CmpPred(p_PtndJoin->pGetChild(i_Child));

    for (i8 i_Kid = p_predCmp->nChildCards() - 1; i_Kid >= 0; i_Kid--) {
      Card *pChild = p_predCmp->pGetChild(i_Kid, eChild);
      if (CK_TERM != eChild)
        continue;

      Card_Term* p_term = _pCard_Term(pChild);
      s32 sTableSyn;
      //E_vColumnRef_TableSyn_x(p_columnRef, sTableSyn);
      sTableSyn.vSprint("%c"_Stz, p_term->sz()->azChar()[1]);

      if (bHasChildWithValue(sTableSyn))
        continue;

      Card_JoinOrder* p_joinOrder = new Card_JoinOrder(sTableSyn);
      vAppend(p_joinOrder);
    }
  }
}


//*===========================================================================*/
/* end                                                                        */
/*============================================================================*/
