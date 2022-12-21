/*============================================================================*/
/* Card_PtndJoin.cpp -- Generate                                              */
/*============================================================================*/

#include "Card_PtndJoin.hpp"

#include "Card_CmpPred.hpp"
#include "Card_Deck.hpp"
#include "Card_Size.hpp"
#include "Card_SubSet.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "CardIter.hpp"

#include "Table.hpp"
#include "StreamDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PtndJoin,CK_PTNDJOIN)
__aNewFunction(Card_PtndJoin,CN_CARD)
__vDeleteFunction(Card_PtndJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PtndJoin::Card_PtndJoin(void)
: Card(CK_PTNDJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PtndJoin::~Card_PtndJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_PtndJoin::vEstimatePtndJoins(Card_Deck* pDeck,
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_PtndJoin::vEstimateJoinBuff(Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nJoinedBuff = 0;

  i8 nTables = p_tableList->nChildCards();
  for (i8 i_Table = 0; i_Table < nTables; i_Table++) {
    Card_Table *p_table =
        _pCard_Table(p_tableList->pGetChild(i_Table));

    if (p_table->Y_bHas(P_TableUsed))
      continue;

    i8 nPredCmps = nChildCards();
    for (i8 i_PredCmp = 0; i_PredCmp < nPredCmps; i_PredCmp++) {
      Card_CmpPred *p_predCmp =
          _pCard_CmpPred(pGetChild(i_PredCmp));

      // todo: ...
//      if (T_bCmpPred_UsesTable(p_predCmp, p_table->cValue())) {
//        nJoinedBuff += p_table->pSub_size()->szValue()->iToI8();
//        p_table->Y_vSet(P_TableUsed);
//      }
    }

  }

  Y_vSet(P_JoinedBuff, nJoinedBuff);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_PtndJoin::bMayFollowAfter(Card_SubSet* pPriorPtnJoins_sofar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPtnJoins_sofar = pPriorPtnJoins_sofar->nChildCards();
  if (! nPtnJoins_sofar)
    return true;

  for (i8 i_PtnJoin_sofar = 0; i_PtnJoin_sofar < nPtnJoins_sofar; i_PtnJoin_sofar++) {
    Card_PtndJoin* p_ptndJoin_sofar =
        _pCard_PtndJoin(pPriorPtnJoins_sofar->pGetChild(i_PtnJoin_sofar));

    i8 nPredCmps_sofar = p_ptndJoin_sofar->nChildCards();
    for (i8 i_PredCmp_sofar = 0; i_PredCmp_sofar < nPredCmps_sofar; i_PredCmp_sofar++) {
      Card_CmpPred* p_predCmp_sofar =
          _pCard_CmpPred(p_ptndJoin_sofar->pGetChild(i_PredCmp_sofar));

      i8 nPredCmps_this = nChildCards();
      for (i8 i_PredCmp_this = 0; i_PredCmp_this < nPredCmps_this; i_PredCmp_this++) {
        Card_CmpPred *p_predCmp_this =
            _pCard_CmpPred(pGetChild(i_PredCmp_this));

        if (p_predCmp_this->bOverlapsWith(p_predCmp_sofar))
          return true;
      }

    }
  }

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
