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
#include "Card_IdxColNm.hpp"
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

    Card_Index*  p_index = new Card_Index;
    p_indexList->vAppend(p_index);

    Card_IndexNm* p_indexNm = new Card_IndexNm;
    p_indexNm->vValue(pIndex->szIndexNm());
    p_index->vAppend(p_indexNm);

    IndexDef* pIndexDef = pIndex->pIndexDef();
    if (pIndexDef->bIsUnique()) {
      p_index->vAppendNewCardTail(CK_UNIQUE);
    }

    Card_IdxColList* p_idxColList = new Card_IdxColList;
    p_index->vAppend(p_idxColList);

    i8 nColumns = pIndexDef->nColumns();
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

      Column* pColumn = pIndexDef->pGetColumn(i_Col);
      Card_IdxCol* p_idxCol = new Card_IdxCol;
      p_idxCol->vValue(pColumn->szColNm());

      p_idxColList->vAppend(p_idxCol);

      Card_IdxColNm* p_idxColNm = new Card_IdxColNm;
      p_idxColNm->vValue(pColumn->szColNm());
      p_idxCol->vAppend(p_idxColNm);
    }
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Index* Card_IndexList::pFindChosenIndex(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (Card_Index* p_index = _pCard_Index(xCI.pEachChild(CK_INDEX))) {
    if (p_index->Y_bHas(P_ChosenIndex))
      return p_index;
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
  while (Card_Index* p_index = _pCard_Index(xCI.pEachChild(CK_INDEX))) {
    if (p_index->pSub_eBound()) {
      p_index->vSet_Choose_index();
      break;
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
  while (Card_Index* p_index = _pCard_Index(xCI.pEachChild(CK_INDEX))) {
    if (p_index->pSub_bounds()) {
      p_index->vSet_Choose_index();
      break;
    }
  }
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
