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
: Card(CK_INDEXLIST)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vDeleteAll_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_INDEX))
    xCI.G.p_index->vDeleteBounds();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
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
/*                                                                            */
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexList::vChoose_index_For_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_INDEX)) {
    if (xCI.G.p_index->pSub_bounds()) {
      xCI.G.p_index->vSet_Choose_index();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_IndexList::bHas_cmpPred_In_bounds(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI_indexList(this);
  while (xCI_indexList.G_bEachMain(CK_INDEX)) {
    CardIter xCI_index(xCI_indexList.G.p_index);
    if (xCI_indexList.G.p_index->bHas_cmpPred_In_bounds(p_cmpPred))
      return true;
  }
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
