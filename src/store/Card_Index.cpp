/*============================================================================*/
/* Card_Index.cpp -- Index                                                    */
/*============================================================================*/

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

__pCastFunction_Card(STORE,Card_Index,CK_INDEX)
__aNewFunction(Card_Index,CN_CARD)
__vDeleteFunction(Card_Index,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Index::Card_Index(void)
: Card(CK_INDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Index::~Card_Index(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce composite <index> of table                                         */
/*----------------------------------------------------------------------------*/
Card_Index* Card_Index::pDerive(Table* pTable)
{
  _static_

  if (!pTable)
    return nullptr;

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Index* pIndex = pIdxVec->pGet(0);

  Card_Index*  p_index = new Card_Index;

  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_indexNm->vValue(pIndex->szIndexNm());
  p_index->vAppend(p_indexNm);

  Card_IdxColList* p_idxColList = new Card_IdxColList;
  p_index->vAppend(p_idxColList);

  IndexDef* pIndexDef = pIndex->pIndexDef();

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

  return p_index;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vSet_Choose_index(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  this->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Index::bTblColOdn_IsKeyIn_index(Stz* szTblColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ASSERT(Card::bIsTableColOrdinal(szTblColOdn));

  Card_IdxColList* p_idxColList = this->pSub_idxColList();

  CardIter xCI(p_idxColList);
  while (Card_IdxCol* p_idxCol = _pCard_IdxCol(xCI.pEachChild())) {

    // ? does n]idxCol:$c.n match given:$c.n
    if (p_idxCol->bSame(szTblColOdn))
      return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
