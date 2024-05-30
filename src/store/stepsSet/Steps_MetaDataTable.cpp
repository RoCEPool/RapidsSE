/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LglPtnMap.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include <new>

#include "Stream.hpp"
#include "Grid.hpp"
#include "CmdLineArgs.hpp"
#include "AttachMap.hpp"
#include "MetaDataTables.hpp"

extern CmdLineArgs  gxCmdLineArgs;
extern AttachMap*   gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaDataTable(void)
{
  vAddStep("MetaDataTable[].select"_Stz,        &Step::vMetaDataTableSelect);
  vAddStep("MetaDataTable[].createTable"_Stz,  &Step::vMetaDataTableCreateTable);
  vAddStep("MetaDataTable[].dropTable"_Stz,  &Step::vMetaDataTableDropTable);
  vAddStep("MetaDataTable[].find"_Stz,  &Step::vMetaDataTableFind);

  vAddStep("MetaDataTable[].createIndex"_Stz,  &Step::vMetaDataTableCreateIndex);
  vAddStep("MetaDataTable[].dropIndex"_Stz,  &Step::vMetaDataTableDropIndex);

  vAddStep("MetaDataTable[].initTables"_Stz,  &Step::vMetaDataTableInitTables);

  vAddStep("MetaDataTable[].insertCatalog"_Stz,  &Step::vMetaDataTableCatalogInsert);
  vAddStep("MetaDataTable[].deleteCatalog"_Stz,  &Step::vMetaDataTableCatalogDelete);

  vAddStep("MetaDataTable[].insertSchema"_Stz,  &Step::vMetaDataTableSchemaInsert);
  vAddStep("MetaDataTable[].deleteSchema"_Stz,  &Step::vMetaDataTableSchemaDelete);

  vAddStep("MetaDataTable[].init"_Stz,  &Step::vMetaDataTableInit);
  vAddStep("MetaDataTable[].setAM"_Stz,  &Step::vMetaDataTableSetAM);
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].select --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSelect(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);

  TablePtn* pTablePtn = pTable->pTablePtn(0);

  BufferId oRootBufferId = pTablePtn->oRootBufferId();
  Pds* pRootPds = _pPds(AM::pHashMap()->pValueAt(oRootBufferId.m_pvAsAddress));

  PdsCursor* pCursor = new PdsCursor(pRootPds);

  while(! pCursor->bIsCurrNull()) {
    SkipListBlock* pSLBlock = pCursor->m_pCurrentBlock;

    BufferId oBufferId = pTablePtn->oExtractBufferId((Object*)pSLBlock->aItem());

    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);

    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);

    pCursor->vNext();
  }
  AM::pHashMap()->vDropKeyLock(oRootBufferId.m_pvAsAddress);
  delete pCursor;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].createTable --                                             */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCreateTable(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  s256 szTableNm;

  eTT eTableType = (eTT) m_pDataStack->iPopInt(__func__);

  Table* pNewTable = _pTable(*m_pLogicUnit->m_xpTable);
  szTableNm = pNewTable->szTableNm();
  i8 iTableAddr = (i8) pNewTable;
  StreamDef* pNewStreamDef = pNewTable->pStreamDef();

  //------------------------------------------------------------------------
  // Insert into tableOfTables
  Table* pTable = AM::pMetaDataTableOfTables();
  if(!pTable) {
    EXIT();
    return;
  }
  StreamDef* pStreamDef = pTable->pStreamDef();
  TablePtn* pTablePtn = pTable->pTablePtn(0);
  Pds* pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  Pds* pPdsRowBuffer = nullptr;
  bool bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                       "rapids"_Stz,
                                       "public"_Stz,
                                       szTableNm.szStz(),
                                       eTableType,
                                       iTableAddr,
                                       C_VARGS_END);

  if(!pRow) {
    if(bAppend) {
      i8 nRows = 0;
      pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
    }

    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);

    // create a new buffer and continue adding rows to it
    pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
    bAppend = true;
    pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    "rapids"_Stz,
                                    "public"_Stz,
                                    szTableNm.szStz(),
                                    C_VARGS_END);
    ASSERT(pRow);
  }

  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }

  //------------------------------------------------------------------------
  // Insert into columns

  Table* pTableColumns = AM::pMetaDataTableColumns();
  StreamDef* pColumnsStreamDef = pTableColumns->pStreamDef();
  pTablePtn = pTableColumns->pTablePtn(0);
  pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  pPdsRowBuffer = nullptr;
  bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pColumnsStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pColumnsStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  i8 nColumns = pNewStreamDef->nColumns();
  i8 i_Col = 0;

  while(i_Col < nColumns) {
    Column* pColumn = pNewStreamDef->pGetColumn(i_Col);

    // Add column info to pds
    Row *pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         "rapids"_Stz,
                                         "public"_Stz,
                                         pNewTable->szTableNm(),
                                         pColumn->iColumnNo(),
                                         pColumn->szColNm(),
                                         (i8)pColumn->eColumnType(),
                                         aCT[pColumn->eColumnType()],
                                         pColumn->iColumnScale(),
                                         pColumn->iColumnLength(),
                                         pColumn->bIsNullable(),
                                         (i8)pColumn->bIsPtnKey(),
                                         C_VARGS_END);

    if (!pRow) {
      if(bAppend) {
        i8 nRows = 0;
        pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
      }

      BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
      AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);

      // create a new buffer and continue adding rows to it
      pPdsRowBuffer = new Pds(pColumnsStreamDef, false, BT_DATA);
      bAppend = true;

      // Add column info to pds
      pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                           "rapids"_Stz,
                                           "public"_Stz,
                                           pNewTable->szTableNm(),
                                           pColumn->iColumnNo(),
                                           pColumn->szColNm(),
                                           (i8)pColumn->eColumnType(),
                                           aCT[pColumn->eColumnType()],
                                           pColumn->iColumnScale(),
                                           pColumn->iColumnLength(),
                                           pColumn->bIsNullable(),
                                           (i8)pColumn->bIsPtnKey(),
                                           C_VARGS_END);
      ASSERT(pRow);
    }
    i_Col++;
  }

  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].createIndex --                                             */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCreateIndex(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  s256 szTableNm;

  Table* pNewTable = _pTable(*m_pLogicUnit->m_xpTable);
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__ ));
  szTableNm = pNewTable->szTableNm();

  //------------------------------------------------------------------------
  // Insert into indexes

  Table* pTableIndexes = AM::pMetaDataTableIndexes();
  StreamDef* pIndexesStreamDef = pTableIndexes->pStreamDef();
  TablePtn* pTablePtn = pTableIndexes->pTablePtn(0);
  Pds* pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  Pds* pPdsRowBuffer = nullptr;
  bool bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pIndexesStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pIndexesStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  // Add index info to pds
  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                       "rapids"_Stz,
                                       "public"_Stz,
                                       szTableNm.szStz(),
                                       pIndex->szIndexNm(),
                                       (i8)pIndex->bIsUnique(),
                                       (i8)pIndex->bIsPrimaryKey(),
                                       C_VARGS_END);

  if (!pRow) {
    if(bAppend) {
      i8 nRows = 0;
      pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
    }

    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);

    // create a new buffer and continue adding rows to it
    pPdsRowBuffer = new Pds(pIndexesStreamDef, false, BT_DATA);
    bAppend = true;
    pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    "rapids"_Stz,
                                    "public"_Stz,
                                    szTableNm.szStz(),
                                    pIndex->szIndexNm(),
                                    C_VARGS_END);
    ASSERT(pRow);
  }


  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }

  //------------------------------------------------------------------------
  // Insert into indexColumns

  Table* pTableIndexCols = AM::pMetaDataTableIndexColumns();
  StreamDef* pIndexColsStreamDef = pTableIndexCols->pStreamDef();
  pTablePtn = pTableIndexCols->pTablePtn(0);
  pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  pPdsRowBuffer = nullptr;
  bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pIndexColsStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pIndexColsStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  i8 nColumns = pIndex->nColumns();
  i8 i_Col = 0;

  while(i_Col < nColumns) {
    Column* pColumn = pIndex->pGetColumn(i_Col);
    // Add index info to pds
    Row *pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         "rapids"_Stz,
                                         "public"_Stz,
                                         szTableNm.szStz(),
                                         pIndex->szIndexNm(),
                                         pColumn->szColNm(),
                                         aCT[pColumn->eColumnType()],
                                         pColumn->iColumnLength(),
                                         pColumn->iColumnScale(),
                                         (i8)pColumn->bIsNullable(),
                                         (i8)pColumn->bIsPtnKey(),
                                         C_VARGS_END);

    if (!pRow) {
      if(bAppend) {
        i8 nRows = 0;
        pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
      }

      BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
      AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);

      // create a new buffer and continue adding rows to it
      pPdsRowBuffer = new Pds(pIndexColsStreamDef, false, BT_DATA);
      bAppend = true;
      pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                      "rapids"_Stz,
                                      "public"_Stz,
                                      szTableNm.szStz(),
                                      pIndex->szIndexNm(),
                                      pIndex->pGetColumn(i_Col)->szColNm(),
                                      i_Col,
                                      C_VARGS_END);
      ASSERT(pRow);
    }
    i_Col++;
  }

  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].dropIndex --                                               */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableDropIndex(void)
{
  ENTRY();

  s256 szTableNm;
  m_pDataStack->vPopStr(szTableNm,__func__);

  s256 szIndexNm;
  m_pDataStack->vPopStr(szIndexNm,__func__);

  //------------------------------------------------------------------------
  // Delete table index info from indexes

  Table* pTable = AM::pMetaDataTableIndexes();
  MetaDataTables::bDeleteRowFromTable(pTable, 2, szTableNm, 3, szIndexNm);

  //------------------------------------------------------------------------
  // Delete table index info from indexColumns

  pTable = AM::pMetaDataTableIndexColumns();
  MetaDataTables::bDeleteRowFromTable(pTable, 2, szTableNm, 3, szIndexNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].initTables -- First step in initializing metaData tables.
 * This is triggered during a define metadata table command.
 * Insert a row in TableOfTables with info about a metadata table.            */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableInitTables(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  s256 szTableNm;

  eTT eTableType = (eTT) m_pDataStack->iPopInt(__func__);

  Table* pNewTable = _pTable(*m_pLogicUnit->m_xpTable);
  i8 iTableAddr = (i8) pNewTable;
  szTableNm = pNewTable->szTableNm();

  Table* pTable = AM::pMetaDataTableOfTables();
  if(!pTable) {
    EXIT();
    return;
  }
  StreamDef* pStreamDef = pTable->pStreamDef();
  TablePtn* pTablePtn = pTable->pTablePtn(0);
  Pds* pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  Pds* pPdsRowBuffer = nullptr;
  bool bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                       "rapids"_Stz,
                                       "public"_Stz,
                                       szTableNm.szStz(),
                                       eTableType,
                                       iTableAddr,
                                       C_VARGS_END);

  if(!pRow) {
    ASSERT(0);
  }

  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }

  AM::pHashMap()->vDropKeyLock(pPdsRootBuffer->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].dropTable --                                               */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableDropTable(void)
{
  ENTRY();

  s256 szTableNm;
  m_pDataStack->vPopStr(szTableNm,__func__);

  //------------------------------------------------------------------------
  // Delete table row info from tableOfTables

  // We first attempt to delete the table row info from tableoftables
  // We check if it's a user defined table or an internal/metadata table
  // If it's not a user table, we ignore the drop table command.

  bool bResult = MetaDataTables::bDeleteFromTableOfTables(szTableNm);

  if(!bResult) {
    EXIT();
    return;
  }

  //------------------------------------------------------------------------
  // Delete table row info from columns

  Table* pTable = AM::pMetaDataTableColumns();

  // More than one row can match this table name, so delete all of them
  MetaDataTables::vDeleteAllRowsFromTable(pTable, 2, szTableNm);

  //------------------------------------------------------------------------
  // Delete table index info from indexes

  pTable = AM::pMetaDataTableIndexes();
  MetaDataTables::vDeleteAllRowsFromTable(pTable, 2, szTableNm);

  //------------------------------------------------------------------------
  // Delete table index info from indexColumns

  pTable = AM::pMetaDataTableIndexColumns();
  MetaDataTables::vDeleteAllRowsFromTable(pTable, 2, szTableNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].find --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableFind(void)
{
  ENTRY();

  s256 szTableNm;
  m_pDataStack->vPopStr(szTableNm,__func__);

  Table* pTable = AM::pMetaDataTableSchema();
  if(!pTable) {
    m_pDataStack->vPushPtr(nullptr);
    EXIT();
    return;
  }

  Table* pNewTable = MetaDataTables::pTableFind(szTableNm);

  m_pDataStack->vPushPtr(pNewTable);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].insertCatalog --                                           */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCatalogInsert(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  s256 szCatalogNm;
  m_pDataStack->vPopStr(szCatalogNm,__func__);

  Table* pTable = AM::pMetaDataTableCatalog();
  if(!pTable) {
    EXIT();
    return;
  }
  StreamDef* pStreamDef = pTable->pStreamDef();
  TablePtn* pTablePtn = pTable->pTablePtn(0);
  Pds* pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  Pds* pPdsRowBuffer = nullptr;
  bool bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                       szCatalogNm.szStz(),
                                       C_VARGS_END);

  if(!pRow) {
    ASSERT(0);
  }

  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }

  AM::pHashMap()->vDropKeyLock(pPdsRootBuffer->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].insertSchema --                                            */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSchemaInsert(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  s256 szSchemaNm;
  m_pDataStack->vPopStr(szSchemaNm,__func__);

  s256 szCatalogNm;
  m_pDataStack->vPopStr(szCatalogNm,__func__);

  Table* pTable = AM::pMetaDataTableSchema();
  if(!pTable) {
    EXIT();
    return;
  }
  StreamDef* pStreamDef = pTable->pStreamDef();
  TablePtn* pTablePtn = pTable->pTablePtn(0);
  Pds* pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  Pds* pPdsRowBuffer = nullptr;
  bool bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                       szCatalogNm.szStz(),
                                       szSchemaNm.szStz(),
                                       C_VARGS_END);

  if(!pRow) {
    ASSERT(0);
  }

  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }

  AM::pHashMap()->vDropKeyLock(pPdsRootBuffer->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].deleteSchema --                                            */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSchemaDelete(void)
{
  ENTRY();

  s256 szCatalogNm;
  m_pDataStack->vPopStr(szCatalogNm,__func__);

  Table* pTable = AM::pMetaDataTableSchema();
  if(!pTable) {
    EXIT();
    return;
  }
  MetaDataTables::vDeleteAllRowsFromTable(pTable, 0, szCatalogNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].deleteCatalog --                                           */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCatalogDelete(void)
{
  ENTRY();

  s256 szCatalogNm;
  m_pDataStack->vPopStr(szCatalogNm,__func__);

  Table* pTable = AM::pMetaDataTableCatalog();
  if(!pTable) {
    EXIT();
    return;
  }
  MetaDataTables::vDeleteAllRowsFromTable(pTable, 0, szCatalogNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].init -- Init all metadata tables.
 * This is called after MetaDataTable[].initTables.
 * This step adds info into metadata tables about the metadata tables themselves*/
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableInit(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  HashMap* pHashMap = AM::pHashMap();

  s256 sCatalogNm = "rapids"_Stz;
  s256 sSchemaNm = "public"_Stz;

  // Iterate the TableOfTables, get the table pointer from each row
  // Add that table column info to metadata columns
  Table* pToT = AM::pMetaDataTableOfTables();
  TablePtn* pToTPtn = pToT->pTablePtn(0);

  BufferId oToTRootBufferId = pToTPtn->oRootBufferId();
  Pds* pToTRootPds = _pPds(AM::pHashMap()->pValueAt(oToTRootBufferId.m_pvAsAddress));

  PdsCursor* pToTBufferIdCursor = new PdsCursor(pToTRootPds);

  while(! pToTBufferIdCursor->bIsCurrNull()) {
    SkipListBlock* pSLBlock = pToTBufferIdCursor->m_pCurrentBlock;

    PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pSLBlock->aItem();

    HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

    Pds* pToTPdsRowBuffer = _pPds(pEntry->m_pValObject);
    PdsCursor* pToTRowCursor = new PdsCursor(pToTPdsRowBuffer);

    // Iterate each pds row buffer in TableOfTables
    while(! pToTRowCursor->bIsCurrNull()) {
      Row* pRow = pToTRowCursor->pCurrRow();

      // Extract the TablePtr
      Table* pRowTable = (Table*)(pRow->iColVal_int(4));
      StreamDef* pRowStreamDef = pRowTable->pStreamDef();


      // Get the Metadata columns table and get it ready for inserting rows
      Table* pColTable = AM::pMetaDataTableColumns();
      StreamDef* pColStreamDef = pColTable->pStreamDef();
      TablePtn* pColTablePtn = pColTable->pTablePtn(0);
      Pds* pColPdsRootBuffer = _pPds(
          AM::pHashMap()->pValueAt(
              pColTablePtn->oRootBufferId().m_pvAsAddress, true
          ));

      Pds* pColPdsRowBuffer = nullptr;
      bool bAppend = false;
      if(pColPdsRootBuffer->nItems() == 0) {
        pColPdsRowBuffer = new Pds(pColStreamDef, false, BT_DATA);
        bAppend = true;
      } else {
        i8 iSearchKey = C_INT_MAX;
        SkipListBlock* pHead = pColPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
        if(pHead != nullptr) {
          // If the block is not null, extract the keys and values
          PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

          HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

          pColPdsRowBuffer = _pPds(pEntry->m_pValObject);
        } else {
          pColPdsRowBuffer = new Pds(pColStreamDef, false, BT_DATA);
          bAppend = true;
        }
      }

      // Get the Table info from the TableOfTables row data
      i8 nColumns = pRowStreamDef->nColumns();
      i8 i_Col = 0;

      while(i_Col < nColumns) {
        Column* pRowColumn = pRowStreamDef->pGetColumn(i_Col);

        // Add column info to pds
        Row *pRow = pColPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                             sCatalogNm.szStz(),
                                             sSchemaNm.szStz(),
                                             pRowTable->szTableNm(),
                                             pRowColumn->iColumnNo(),
                                             pRowColumn->szColNm(),
                                             (i8)pRowColumn->eColumnType(),
                                             aCT[pRowColumn->eColumnType()],
                                             pRowColumn->iColumnScale(),
                                             pRowColumn->iColumnLength(),
                                             pRowColumn->bIsNullable(),
                                             (i8)pRowColumn->bIsPtnKey(),
                                             C_VARGS_END);

        if (!pRow) {
          if(bAppend) {
            i8 nRows = 0;
            pColTablePtn->vAppendBuffer(pColPdsRowBuffer, nRows);
          }

          BufferId oBufferId = pColPdsRowBuffer->oGetBufferId().m_pvAsAddress;
          AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);

          // create a new buffer and continue adding rows to it
          pColPdsRowBuffer = new Pds(pColStreamDef, false, BT_DATA);
          bAppend = true;
          // Add column info to pds
          pRow = pColPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                          sCatalogNm.szStz(),
                                          sSchemaNm.szStz(),
                                          pRowTable->szTableNm(),
                                          pRowColumn->iColumnNo(),
                                          pRowColumn->szColNm(),
                                          (i8)pRowColumn->eColumnType(),
                                          aCT[pRowColumn->eColumnType()],
                                          pRowColumn->iColumnScale(),
                                          pRowColumn->iColumnLength(),
                                          pRowColumn->bIsNullable(),
                                          (i8)pRowColumn->bIsPtnKey(),
                                          C_VARGS_END);
          ASSERT(pRow);
        }
        i_Col++;
      }

      if(bAppend) {
        i8 nRows = 0;
        pColTablePtn->vAppendBuffer(pColPdsRowBuffer, nRows);
      } else {
        AM::pHashMap()->vDropKeyLock(pColPdsRowBuffer->oGetBufferId().m_pvAsAddress);
      }




      pToTRowCursor->vNext();
    }

    pHashMap->vDropKeyLock(pToTPdsRowBuffer->oGetBufferId().m_pvAsAddress);

    pToTBufferIdCursor->vNext();
    delete pToTRowCursor;
  }
  AM::pHashMap()->vDropKeyLock(oToTRootBufferId.m_pvAsAddress);
  delete pToTBufferIdCursor;

  //------------------------------------------------------------------------
  // Insert into nodes

  Table* pTableNodes = AM::pMetaDataTableNodes();
  StreamDef* pNodesStreamDef = pTableNodes->pStreamDef();
  TablePtn* pTablePtn = pTableNodes->pTablePtn(0);
  Pds* pPdsRootBuffer = _pPds(
      AM::pHashMap()->pValueAt(
          pTablePtn->oRootBufferId().m_pvAsAddress, true
      ));

  Pds* pPdsRowBuffer = nullptr;
  bool bAppend = false;
  if(pPdsRootBuffer->nItems() == 0) {
    pPdsRowBuffer = new Pds(pNodesStreamDef, false, BT_DATA);
    bAppend = true;
  } else {
    i8 iSearchKey = C_INT_MAX;
    SkipListBlock* pHead = pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress, true);

      pPdsRowBuffer = _pPds(pEntry->m_pValObject);
    } else {
      pPdsRowBuffer = new Pds(pNodesStreamDef, false, BT_DATA);
      bAppend = true;
    }
  }

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    UNUSED(pNode);

    Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         (*oNode_I)->iNodeId(),
                                         (*oNode_I)->szIPAddr(),
                                         (*oNode_I)->iPortNo(),
                                         0,
                                         C_VARGS_END);
    if (!pRow) {
      if(bAppend) {
        i8 nRows = 0;
        pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
      }

      BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
      AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);

      // create a new buffer and continue adding rows to it
      pPdsRowBuffer = new Pds(pNodesStreamDef, false, BT_DATA);
      bAppend = true;
      pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                      (*oNode_I)->iNodeId(),
                                      (*oNode_I)->szIPAddr(),
                                      (*oNode_I)->iPortNo(),
                                      0,
                                      C_VARGS_END);
      ASSERT(pRow);
    }
  }

  if(bAppend) {
    i8 nRows = 0;
    pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
  } else {
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].setAM --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSetAM(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);

  AM::ATTACHMAP::vMetaDataTables(pTable);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
