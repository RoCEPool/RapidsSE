/*============================================================================*/
/* Card_Index.cpp -- Index                                                    */
/*============================================================================*/

#include "Card_Index.hpp"

#include "Table.hpp"
#include "Card_IndexNm.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"
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
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull non-used bound                                                        */
/*----------------------------------------------------------------------------*/
void Card_Index::vCullNonusedBounds(CardIter* pCI) {

  _static_

  // find & ignore the first ebound
  //bool bFoundFirst = false;

  CardIter xCI_indexes(pCI->G.p_indexList);

  while (xCI_indexes.G_bEachMain(CK_INDEX)) {

    bool bChosenIndex = xCI_indexes.G.p_index->pSymbol()->Y_bHas(P_ChosenIndex);

    xCI_indexes.G.p_bounds = xCI_indexes.G.p_index->pSub_bounds();
    if (! xCI_indexes.G.p_bounds)
      continue;

    CardIter xCI_bounds(xCI_indexes.G.p_bounds);
    while (xCI_bounds.G_bEachMain(CK_BOUND)) {
      if (xCI_bounds.G.p_bound->pSymbol()->Y_bHas(P_ChosenBound)) {
        continue;
      }

      xCI_bounds.G.p_MainCard->vMark1(MARK_AT);
    }

    pCI->vPrintBoard(__LINE__);
    xCI_indexes.G.p_bounds->vCullBound(MARK_AT);

  }

}

/*----------------------------------------------------------------------------*/
/* produce composite <index> of table                                         */
/*----------------------------------------------------------------------------*/
Card_Index* Card_Index::pDerive(Index* pIndex)
{
  _static_

  Card_Index* p_index = new Card_Index;
  IndexDef* pIndexDef = pIndex->pIndexDef();

  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_indexNm->vSymbol(pIndex->szIndexNm());
  p_index->vAppend(p_indexNm);

  if (pIndexDef->bIsUnique()) {
    Card_Unique* p_unique = new Card_Unique;
    p_index->vAppend(p_unique);
  }

  Card_IdxColList* p_idxColList = new Card_IdxColList;
  p_index->vAppend(p_idxColList);

  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    Card_IdxCol* p_idxCol = new Card_IdxCol;
    p_idxCol->vSymbol(pColumn->szColNm());

    p_idxColList->vAppend(p_idxCol);

//    Card_IdxColNm* p_idxColNm = new Card_IdxColNm;
//    p_idxColNm->vSymbol(pColumn->szColNm());
//    p_idxCol->vAppend(p_idxColNm);
  }

  return p_index;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vAdd_key_exprs(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_idxColList = pSub_idxColList();

  pCI->G.p_idxColList->vAdd_key_exprs(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vDeleteBounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pSymbol()->Y_vCut(P_ChosenIndex);
  Card_Bounds* p_Bounds = pSub_bounds();
  vDeleteChild(p_Bounds);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Index::bHas_cmpPred_In_bounds(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI_index(this);
  while (xCI_index.G_bEachMain(CK_BOUNDS)) {
    if (xCI_index.G.p_bounds->bHasBoundOf(p_cmpPred))
      return true;
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Index::vSet_Choose_index(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  this->pSymbol()->Y_vSet(P_ChosenIndex);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Index::bTblColOdn_IsKeyIn_index(Stz* szTblColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColList* p_idxColList = this->pSub_idxColList();

  CardIter xCI(p_idxColList);
  while (xCI.G_bEachMain(CK_IDXCOL)) {

    // ? does n]idxCol:$c.n match given:$c.n
    if (xCI.G.p_idxCol->bIsSymbol(szTblColOdn))
      return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
