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

  p_joinOrdering->vMorphChildrenTo(CK_STREAMHEAD, CK_JOINORDER);
  p_joinOrdering->vMorphChildrenTo(CK_JOINLOCUS, CK_JOINORDER);

  return p_joinOrdering;
}

/*----------------------------------------------------------------------------*/
/* pick optimal join ordering from join permutation set                       */
/*----------------------------------------------------------------------------*/
Card_JoinOrdering* Card_JoinOrdering::pPick(Card_JoinPermAll* p_joinPermAll, i8 iJoinOrder)
{
  _static_

  Card_JoinPermSeq* p_joinPermSeq = p_joinPermAll->pJoinOrder(iJoinOrder);

  // is it a valid join order ?
  if ( ! p_joinPermSeq )
    return nullptr;

  Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(p_joinPermSeq);

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
void Card_JoinOrdering::vJoinOrderString(String* pString,
                                         Card_TableList* p_tableList,
                                         bool bNumbers)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CostQuery*   p_costQuery   = pSub_costQuery();
  Card_CostNetwork* p_costNetwork = p_costQuery->pSub_costNetwork();
  Card_CostUnicast* p_costUnicast = p_costQuery->pSub_costUnicast();

  if (bNumbers) {
    pString->vSprint("JO %5lld "_Stz,pSymbol()->Y_iGet(P_Permutation));
  }

  // the join sequence
  CardIter xCI(this);
  if (xCI.G_bEachMain(CK_JOINORDER)) {
    char cHeadTable = xCI.G.p_joinOrder->cValue();
    s256 sHeadTable = p_tableList->szToTableNm(cHeadTable);

    if (bNumbers)
      pString->vCatSprint("NetCost %5s UniCost %5s : %8s"_Stz,
                          p_costNetwork->szValue(),
                          p_costUnicast->szValue(),
                          sHeadTable.szStz());
    else
      pString->vSprint("%s"_Stz,sHeadTable.szStz());
  }

  while (xCI.G_bEachMain(CK_JOINORDER)) {
    char cNextTable = xCI.G.p_joinOrder->cValue();
    s256 sNextTable = p_tableList->szToTableNm(cNextTable);
    xCI.G.p_costLocus = xCI.G.p_joinOrder->pSub_costLocus();
    xCI.G.p_locusCast = xCI.G.p_costLocus->pSub_locusCast();
    xCI.G.p_costUnicast = xCI.G.p_costLocus->pSub_costUnicast();
    xCI.G.p_costNetwork = xCI.G.p_costLocus->pSub_costNetwork();
    xCI.G.p_costJoining = xCI.G.p_costLocus->pSub_costJoining();
    s32 sCast = xCI.G.p_locusCast->szValue();
    StrLib::vShuffleLeft(sCast.azChar(),3);

    if (bNumbers)
      pString->vCatSprint(" %5s %9s %8s (%5s)"_Stz,
                          sCast.bSame("UNICAST"_Stz)
                            ? xCI.G.p_costUnicast->szValue()
                            : xCI.G.p_costNetwork->szValue(),
                          sCast.szStz(),
                          sNextTable.szStz(),
                          xCI.G.p_costJoining->szValue());
    else
      pString->vCatSprint(" %s %s"_Stz,sCast.szStz(),
                          sNextTable.szStz());
  }

  // the load
//  Card_CostQuery* p_costQuery = pSub_costQuery();
//  Card_CostNetwork* p_costNetwork = p_costQuery->pSub_costNetwork();
//  Card_CostUnicast* p_costUnicast = p_costQuery->pSub_costUnicast();
//
//  pString->vCatSprint(", %lld, %lld"_Stz, p_costNetwork->iValue(), p_costUnicast->iValue());
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
