/*============================================================================*/
/* Card_JoinOrdering.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinOrdering.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Deck.hpp"
#include "Card_JoinOrder.hpp"
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
Card_JoinOrdering* Card_JoinOrdering::pDerive(Card_JoinPermSeq* p_joinPermSeq)
{
  _static_

  s32 sTableSyn;

  Card_JoinPermSeq* p_joinPermSeq_dup =
      _pCard_JoinPermSeq(p_joinPermSeq->pDupCardRecursive());

  p_joinPermSeq_dup->vCard(CK_JOINORDERING);

  Card_JoinOrdering* p_joinOrdering = _pCard_JoinOrdering(p_joinPermSeq_dup);

  p_joinOrdering->vMorphChildrenTo(CK_DATASTART, CK_JOINORDER);
  p_joinOrdering->vMorphChildrenTo(CK_JOINNEXT,CK_JOINORDER);

  return p_joinOrdering;
}

/*----------------------------------------------------------------------------*/
/* pick optimal join ordering from join permutation set                       */
/*----------------------------------------------------------------------------*/
Card_JoinOrdering* Card_JoinOrdering::pPick(Card_JoinPermAll* p_joinPermAll)
{
  _static_

  Card_JoinPermSeq* p_joinPermSeq_lowest = p_joinPermAll->pLowestLoad();

  Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(p_joinPermSeq_lowest);

  return p_joinOrdering;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* derive the join order string                                               */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vJoinOrderString(String* pString, Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // the join sequence
  CardIter xCI(this);
  if (xCI.G_bEachMain(CK_JOINORDER)) {
    char cHeadTable = xCI.G.p_joinOrder->cValue();
    s256 sHeadTable = p_tableList->szToTableNm(cHeadTable);

    pString->vSprint("%s"_Stz,sHeadTable.szStz());
  }

  while (xCI.G_bEachMain(CK_JOINORDER)) {
    char cNextTable = xCI.G.p_joinOrder->cValue();
    s256 sNextTable = p_tableList->szToTableNm(cNextTable);
    xCI.G.p_costNext = xCI.G.p_joinOrder->pSub_costNext();
    xCI.G.p_cast = xCI.G.p_costNext->pSub_castNext();
    s32 sCast = xCI.G.p_cast->szValue();
    StrLib::vShuffleLeft(sCast.azChar(),3);

    pString->vCatSprint(" %s %s"_Stz,sCast.szStz(), sNextTable.szStz());
  }

  // the load
  Card_CostQuery* p_costQuery = pSub_costQuery();
  Card_CostNetwork* p_costNetwork = p_costQuery->pSub_costNetwork();
  Card_CostUnicast* p_costUnicast = p_costQuery->pSub_costUnicast();

  pString->vCatSprint(", %lld, %lld"_Stz, p_costNetwork->iValue(), p_costUnicast->iValue());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vReorder(Card_Deck* pDeck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pAfter = nullptr;

  {
    CardIter xCI_deck(pDeck);
    if (xCI_deck.G_bEachMain(CK_FOLD))
      pAfter = xCI_deck.G.p_fold;

    if (! pAfter) {
      xCI_deck.vRestart();
      if (xCI_deck.G_bEachMain(CK_CORRELATION))
        pAfter = xCI_deck.G.p_correlation;
    }
  }

  CardIter xCI_joinOrder(this);
  while (xCI_joinOrder.G_bEachMain(CK_JOINORDER)) {

    Stz* szTableSyn = xCI_joinOrder.G.p_joinOrder->szValue();

    i8 i_Table = C_UNKNOWN;
    Card_Table* p_table =
        _pCard_Table(pDeck->pFindChild(CK_TABLE,szTableSyn,i_Table));

    pDeck->vRemoveChild(p_table);

    if (pAfter)
      pDeck->vInsertAfter(p_table,pAfter);
    else
      pDeck->vPush(p_table);

    pAfter = p_table;
  }

}

//*===========================================================================*/
/* end                                                                        */
/*============================================================================*/
