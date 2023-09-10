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
: Card_symbol(CK_JOINORDERING, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  vAddedCorrectly();

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
Card_JoinOrdering* Card_JoinOrdering::pDerive(Card_StreamPer* p_streamPer)
{
  _static_

  s32 sTableSyn;

  Card_StreamPer* p_joinPermSeq_dup =
      _pCard_StreamPer(p_streamPer->pDupCardRecursive());

  p_joinPermSeq_dup->vCard(CK_JOINORDERING);
  p_joinPermSeq_dup->vAddedCorrectly();

  Card_JoinOrdering* p_joinOrdering = _pCard_JoinOrdering(p_joinPermSeq_dup);

  p_joinOrdering->vMorphChildrenTo(CK_STREAMHEAD, CK_JOINORDER);
  p_joinOrdering->vMorphChildrenTo(CK_STREAMJOIN, CK_JOINORDER);

  return p_joinOrdering;
}

/*----------------------------------------------------------------------------*/
/* pick optimal join ordering from join permutation set                       */
/*----------------------------------------------------------------------------*/
Card_JoinOrdering* Card_JoinOrdering::pPick(Card_StreamSet* p_joinPermAll, i8 iJoinOrder)
{
  _static_

  Card_StreamPer* p_joinPermSeq = p_joinPermAll->pJoinOrder(iJoinOrder);

  // is it a valid join order ?
  if ( ! p_joinPermSeq )
    return nullptr;

  Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(p_joinPermSeq);

  return p_joinOrdering;
}

/*----------------------------------------------------------------------------*/
/* pick optimal join ordering from join permutation set                       */
/*----------------------------------------------------------------------------*/
Card_JoinOrdering* Card_JoinOrdering::pPick(Card_StreamSet* p_joinPermAll, bool bLowest)
{
  _static_

  Card_StreamPer* p_joinPermSeq_picked;

  if (bLowest)
    p_joinPermSeq_picked = p_joinPermAll->pLowestCost();
  else
    p_joinPermSeq_picked = p_joinPermAll->pHighestCost();

  if (! p_joinPermSeq_picked)
    return nullptr;

  Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(p_joinPermSeq_picked);

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

  Card_CostStream*   p_costQuery   = pSub_costQuery();
  Card_CostNetwork* p_costNetwork = p_costQuery->pSub_costNetwork();
  Card_CostUnicast* p_costUnicast = p_costQuery->pSub_costUnicast();

  if (bNumbers) {
    pString->vSprint("JO %5lld "_Stz,pSymbol()->Y_iGet(P_Permutation));
  }

  // the join sequence
  Kid xKid(this);
  if (xKid.bEach(CK_JOINORDER)) {
    char cHeadTable = xKid.p_joinOrder()->pSymbol()->cSF_char();
    s256 sHeadTable = p_tableList->szToTableNm(cHeadTable);

    if (bNumbers)
      pString->vCatSprint("NetCost %5s UniCost %5s : %8s"_Stz,
                          p_costNetwork->szSymbol(SF_NONE),
                          p_costUnicast->szSymbol(SF_NONE),
                          sHeadTable.szStz());
    else
      pString->vSprint("%s"_Stz,sHeadTable.szStz());
  }

  while (xKid.bEach(CK_JOINORDER)) {
    Card_JoinOrder* p_joinOrder = xKid.p_joinOrder();
    char cNextTable = p_joinOrder->pSymbol()->cSF_char();
    s256 sNextTable = p_tableList->szToTableNm(cNextTable);
    Card_JoinEstimate* p_joinEstimate = p_joinOrder->pSub_joinEstimate();
    Card_JoinCast* p_joinCast = p_joinEstimate->pSub_locusCast();
    Card_CostUnicast* p_costUnicast = p_joinEstimate->pSub_costUnicast();
    Card_CostNetwork* p_costNetwork = p_joinEstimate->pSub_costNetwork();
    Card_CostJoining* p_costJoining = p_joinEstimate->pSub_costJoining();
    s32 sCast = p_joinCast->szSymbol(SF_NONE);
    StrLib::vShuffleLeft(sCast.azChar(), sCast.nAreaBytes(), 3);

    if (bNumbers)
      pString->vCatSprint(" %5s %9s %8s (%5s)"_Stz,
                          sCast.bSame("UNICAST"_Stz)
                            ? p_costUnicast->szSymbol(SF_NONE)
                            : p_costNetwork->szSymbol(SF_NONE),
                          sCast.szStz(),
                          sNextTable.szStz(),
                          p_costJoining->szSymbol(SF_NONE));
    else
      pString->vCatSprint(" %s %s"_Stz,sCast.szStz(),
                          sNextTable.szStz());
  }

  // the load
//  Card_CostStream* p_costQuery = pSub_costQuery();
//  Card_CostNetwork* p_costNetwork = p_costQuery->pSub_costNetwork();
//  Card_CostUnicast* p_costUnicast = p_costQuery->pSub_costUnicast();
//
//  pString->vCatSprint(", %lld, %lld"_Stz, p_costNetwork->iValue(), p_costUnicast->pSymbol()->iSF_number());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vReorder(Card_Deck* pDeck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pAfter = nullptr;

  {
    Kid xCI_deck(pDeck);
    if (xCI_deck.bEach(CK_FOLD))
      pAfter = xCI_deck.p_fold();

    if (! pAfter) {
      xCI_deck.vEnded();
      xCI_deck.vBeginEach_forward();
      if (xCI_deck.bEach(CK_CORRELATION))
        pAfter = xCI_deck.p_correlation();
    }
  }

  Kid xCI_joinOrder(this);
  while (xCI_joinOrder.bEach(CK_JOINORDER)) {

    Stz* szTableSyn = xCI_joinOrder.p_joinOrder()->szSymbol(SF_NONE);

    i8 i_Table = C_UNKNOWN;
    Card_From* p_from =
        _pCard_From(pDeck->pFindChildWithSub(CK_FROM,CK_TABLE,szTableSyn,i_Table));

    pDeck->vRemoveChild(p_from);

    if (pAfter)
      pDeck->vInsertChildAfter(p_from,pAfter);
    else
      pDeck->vPushChild(p_from);

    pAfter = p_from;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinOrdering::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*===========================================================================*/
/* end                                                                        */
/*============================================================================*/
