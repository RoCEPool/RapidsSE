/*============================================================================*/
/* Card_IndexPtr.cpp -- Index                                                    */
/*============================================================================*/

#include "Card_IdxColList.hpp"
#include "Card_IndexPtr.hpp"

#include "CardCtl.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_IndexPtr, CK_INDEXPTR)
__aNewFunction(Card_IndexPtr, CN_CARD)
__vDeleteFunction(Card_IndexPtr, CN_CARD)

__bIsCard(STORE, Card_IndexPtr, CK_INDEXPTR)
__pSubFunction_Card(STORE,IndexPtr, CK_INDEXPTR)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexPtr::Card_IndexPtr(Index* pIndex)
: Card(CK_INDEXPTR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

   vIndex(pIndex);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexPtr::~Card_IndexPtr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_IdxColNm* Card_IndexPtr::pFind_indexCol(Stz *szColNm, i8& i_IdxCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IdxColList* p_idxColList = this->pSub_idxColList();

  Card_IdxColNm* p_idxCol = p_idxColList->pFind_idxCol(szColNm, i_IdxCol);

  return p_idxCol;
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Index* Card_IndexPtr::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Index* pIndex = _pIndex(pPpy(PPY_Index));

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* set index                                                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexPtr::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr(PPY_Index,pIndex);
}


/*----------------------------------------------------------------------------*/
/* print {index}                                                              */
/*----------------------------------------------------------------------------*/
void Card_IndexPtr::vIndexPtrPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());
  if (! pCardCtl->iPtrDetail(CK_INDEXPTR))
    return;

  Card_IndexPtr* pCard_IndexPtr = _pCard_IndexPtr(this);
  Index* pIndex = pCard_IndexPtr->pIndex();
  IndexDef* pIndexDef = pIndex->pIndexDef();
  Table* pTable = pIndex->pTable();

  pRibbon->vSput(FR_NAME,pIndex->szIndexNm());

  pRibbon->vSput(FR_TEXT
      ,pIndexDef->bIsUnique()
       ? "unique"_Stz : "not unique"_Stz
  );

  pRibbon->vSput(FR_NAME,pTable->szTableNm());

  i8 nIdxCols = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nIdxCols; i_Col++) {
    Column* pIdxCol = pIndexDef->pGetColumn(i_Col);

    pRibbon->vSput(FR_ATOM_SQUARE,pIdxCol->szColNm());
  }

  i8 nBuffers = 0;
  i8 nEntries = 0;
  pIndex->vAnalyze(nBuffers,nEntries);
  pRibbon->vSprintRight(" %lld buffers"_Stz,nBuffers);
  pRibbon->vSprintRight(" %lld entries"_Stz,nEntries);

  //vPrintProperties(pRibbon,iDepth);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
