/*============================================================================*/
/* Card_IndexList.cpp -- Index List                                           */
/*============================================================================*/

#include "Card_IndexList.hpp"

#include "Index.hpp"
#include "Card_Index.hpp"
#include "Table.hpp"
#include "Card_IndexNm.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexList,CK_INDEXLIST)
__aNewFunction(Card_IndexList,CN_CARD)
__vDeleteFunction(Card_IndexList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexList::Card_IndexList(void)
: Card_symbol(CK_INDEXLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexList::~Card_IndexList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce composite <indexList> of table                                     */
/*----------------------------------------------------------------------------*/
Card_IndexList* Card_IndexList::pDerive(Table* pTable)
{
  _static_

  if (!pTable)
    return nullptr;

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Card_IndexList* p_indexList = new Card_IndexList;

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);

    Card_Index* p_index = Card_Index::pDerive(pIndex);
    p_indexList->vAppend(p_index);
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete all bounds (within this indexList)                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vDeleteAll_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_INDEX))
    xCI.G.p_index->vDeleteBounds();
}


/*----------------------------------------------------------------------------*/
/* find chosen index                                                          */
/*----------------------------------------------------------------------------*/
Card_Index* Card_IndexList::pFindChosenIndex(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_INDEX)) {
    if (xCI.G.p_index->pSymbol()->Y_bHas(P_ChosenIndex))
      return xCI.G.p_index;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* choose appropriate index for crossJoin                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vChoose_index_For_crossJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_INDEX)) {
    xCI.G.p_bounds = xCI.G.p_index->pSub_bounds();
    if (! xCI.G.p_bounds)
      continue;
    xCI.G.p_bound = xCI.G.p_bounds->pSub_bound();
    if (! xCI.G.p_bound)
      continue;
    xCI.G.p_index->vSet_Choose_index();
    xCI.G.p_bound->pSymbol()->Y_vSet(P_ChosenBound);
    return;
  }

}

/*----------------------------------------------------------------------------*/
/* choose appropriate index for indexJoin                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vChoose_index_For_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_INDEX)) {
    if (! xCI.G.p_index->pSub_bounds())
      continue;
    CardIter xCI_bounds(xCI.G.p_index->pSub_bounds());
    while (xCI_bounds.G_bEachMain(CK_BOUND)) {
      if (xCI_bounds.G.p_bound->bIsEBound()) {
        xCI.G.p_index->vSet_Choose_index();
        xCI_bounds.G.p_bound->pSymbol()->Y_vSet(P_ChosenBound);
        return;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* choose appropriate index for indexScan                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vChoose_index_For_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_INDEX)) {
    if (! xCI.G.p_index->pSub_bounds())
      continue;
    CardIter xCI_bounds(xCI.G.p_index->pSub_bounds());
    while (xCI_bounds.G_bEachMain(CK_BOUND)) {
      if (xCI_bounds.G.p_bound->bIsLBound() ||
          xCI_bounds.G.p_bound->bIsUBound()) {
        xCI.G.p_index->vSet_Choose_index();
        xCI_bounds.G.p_bound->pSymbol()->Y_vSet(P_ChosenBound);
        return;
      }
    }
  }

}
/*----------------------------------------------------------------------------*/
//void Card_IndexList::vChoose_index_For_indexScan(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
//
//  CardIter xCI(this);
//  while (xCI.G_bEachMain(CK_INDEX)) {
//    if (xCI.G.p_index->pSub_bounds()) {
//      xCI.G.p_index->vSet_Choose_index();
//      break;
//    }
//  }
//}

/*----------------------------------------------------------------------------*/
/* does this index-list have a bound logically equivalent to given compare?   */
/*----------------------------------------------------------------------------*/
bool Card_IndexList::bHasEquivalentBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xIndexList(this);
  while (xIndexList.bEach(CK_INDEX)) {
    if (xIndexList.p_index()->bHasEquivalentBound(p_cmpPred))
      return true;
  }
  return false;
}

/*----------------------------------------------------------------------------*/
/* does an index in this indexList use sql result?                            */
/*----------------------------------------------------------------------------*/
bool Card_IndexList::bUsesSqyRsl(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_INDEX); /**/) {
    if (xKid.p_index()->bUsesSqlRsl())
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* ensure types for all stream-columns                                        */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vTypes_For_stmColOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_INDEX); /**/)
    xKid.p_index()->vEnsureTypes_For_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  i8 nChosenIndexes = 0;

  for (Kid xCI(this); xCI.bEach(CK_INDEX); /**/) {
    pCI->G.p_index = xCI.p_index();
    pCI->G.p_index->vValidate(pCI, iDepth + 1, this);
    if (pCI->G.p_index->pSymbol()->Y_bHas(P_ChosenIndex))
      nChosenIndexes++;
  }

  RASSERT(nChosenIndexes <= 1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
