/*============================================================================*/
/* Card_TablePtr.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_TablePtr.hpp"

#include "Card_IndexPtr.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexList.hpp"

#include "CardCtl.hpp"

#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_TablePtr, CK_TABLEPTR)
__aNewFunction(Card_TablePtr, CN_CARD)
__vDeleteFunction(Card_TablePtr, CN_CARD)

__bIsCard(STORE, Card_TablePtr, CK_TABLEPTR)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtr::Card_TablePtr(Table* pTable)
: Card(CK_TABLEPTR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vTable(pTable);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TablePtr::~Card_TablePtr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_IndexList* Card_TablePtr::pNew_indexList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Table* pTable = this->pTable();

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Card_IndexList* p_indexList = new Card_IndexList;

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);

    Card_IndexPtr* pCard_index = new Card_IndexPtr(pIndex);
    p_indexList->vAppend(pCard_index);

    Card_IdxColList* pCard_idxColList = new Card_IdxColList(pIndex);
    pCard_index->vAppend(pCard_idxColList);
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Table* Card_TablePtr::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Table* pTable = _pTable(pPpy(PPY_Table));

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Card_TablePtr::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vPpy_ptr(PPY_Table,pTable);
}

/*----------------------------------------------------------------------------*/
/* print {table}                                                              */
/*----------------------------------------------------------------------------*/
void Card_TablePtr::vTablePtrPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());
  if (! pCardCtl->iPtrDetail(CK_TABLEPTR))
    return;

  Card_TablePtr* pCardTablePtr = _pCard_TablePtr(this);
  Table* pTable = _pTable(pCardTablePtr->pTable());

  //pRibbon->vSput(FR_NAME,pTable->szTableNm());

  if (pRibbon->iArea())
    return;

  //pCardTablePtr->vPrintPartition(pRibbon);
  PtnDef* pPtnDef = pTable->pPtnDef();
  Column* pColumn = pPtnDef->pGetColumn(0);

  pRibbon->vSput(FR_ATOM_SLASH,pColumn->szColNm());


  //pCardTablePtr->vPrintSize(pRibbon);
  StreamDef* pStreamDef = pTable->pStreamDef();

  //pCardTablePtr->vPrintPrimary(pRibbon);
  IdxVec*   pIdxVec    = pTable->pIdxVec();
//  Index*    pIndex     = pIdxVec->pGet(0);
//  IndexDef* pIndexDef  = pIndex->pIndexDef();
//  Column*   pIdxColumn = pIndexDef->pGetColumn(0);
//   pRibbon->vSput(FR_ATOM_SQUARE,pIdxColumn->szColNm());

  //pRibbon->vSput(FR_SIZE,pStreamDef->jTableSize());
  pRibbon->vSprintRight(" %lld %s"_Stz,pStreamDef->nColumns(),
                        1 == pStreamDef->nColumns() ? "column" : "columns");
  pRibbon->vSprintRight(" %lld %s"_Stz,pIdxVec->nItems(),
                        1 == pIdxVec->nItems() ? "index" : "indexes");
  pRibbon->vSprintRight(" tableSize %lld"_Stz,pStreamDef->jTableSize());
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
StreamDef* Card_TablePtr::pStreamDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Table *pTable = this->pTable();
  if (! pTable)
    return nullptr;

  StreamDef* pStreamDef = pTable->pStreamDef();

  return pStreamDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
