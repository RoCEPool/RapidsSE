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

  vAddStep("MetaDataTable[].createIndex"_Stz,  &Step::vMetaDataTableCreateIndex);
  vAddStep("MetaDataTable[].dropIndex"_Stz,  &Step::vMetaDataTableDropIndex);

  vAddStep("MetaDataTable[].insertTables"_Stz,  &Step::vMetaDataTableTablesInsert);

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
    // Add index info to pds
    Row *pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         "rapids"_Stz,
                                         "public"_Stz,
                                         szTableNm.szStz(),
                                         pIndex->szIndexNm(),
                                         pIndex->pGetColumn(i_Col)->szColNm(),
                                         i_Col,
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
  MetaDataTables::vDeleteRowFromTable(pTable, 2, szTableNm);

  //------------------------------------------------------------------------
  // Delete table index info from indexColumns

  pTable = AM::pMetaDataTableIndexColumns();
  MetaDataTables::vDeleteRowFromTable(pTable, 2, szTableNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].insertTables --                                            */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableTablesInsert(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  s256 szTableNm;
//  m_pDataStack->vPopStr(szTableNm,__func__);

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

  Table* pTable = AM::pMetaDataTableOfTables();

  // This fn deletes all rows from the table
  // We KNOW that there should exist only one row with this table name
  // TODO: Arjun - Break out of searching for the table name once we found
  // a match.
  MetaDataTables::vDeleteRowFromTable(pTable, 2, szTableNm);

  //------------------------------------------------------------------------
  // Delete table row info from columns

  pTable = AM::pMetaDataTableColumns();

  // More than one row can match this table name, so delete all of them
  MetaDataTables::vDeleteRowFromTable(pTable, 2, szTableNm);

  //------------------------------------------------------------------------
  // Delete table index info from indexes

  pTable = AM::pMetaDataTableIndexes();
  MetaDataTables::vDeleteRowFromTable(pTable, 2, szTableNm);

  //------------------------------------------------------------------------
  // Delete table index info from indexColumns

  pTable = AM::pMetaDataTableIndexColumns();
  MetaDataTables::vDeleteRowFromTable(pTable, 2, szTableNm);

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
  MetaDataTables::vDeleteRowFromTable(pTable, 0, szCatalogNm);

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
  MetaDataTables::vDeleteRowFromTable(pTable, 0, szCatalogNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].init -- Init all metadata tables                           */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableInit(void)
{
  ENTRY();

  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  for(i8 iMetaDataTable = 1; iMetaDataTable < (i8) MD_ENUMS; iMetaDataTable++) {
    Table* pTableCur = AM::ATTACHMAP::pMetaDataTables(aMD[iMetaDataTable]);
    if(!pTableCur)
      continue;
    StreamDef* pStreamDef = pTableCur->pStreamDef();

    s256 sCatalogNm = "rapids"_Stz;
    s256 sSchemaNm = "public"_Stz;


    //------------------------------------------------------------------------
    // Insert into columns

    Table* pTableColumns = AM::pMetaDataTableColumns();
    StreamDef* pColumnsStreamDef = pTableColumns->pStreamDef();
    TablePtn* pTablePtn = pTableColumns->pTablePtn(0);
    Pds* pPdsRootBuffer = _pPds(
        AM::pHashMap()->pValueAt(
            pTablePtn->oRootBufferId().m_pvAsAddress, true
        ));

    Pds* pPdsRowBuffer = nullptr;
    bool bAppend = false;
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

    i8 nColumns = pStreamDef->nColumns();
    i8 i_Col = 0;

    while(i_Col < nColumns) {
      Column* pColumn = pStreamDef->pGetColumn(i_Col);

      // Add column info to pds
      Row *pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                           sCatalogNm.szStz(),
                                           sSchemaNm.szStz(),
                                           pTableCur->szTableNm(),
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
                                             sCatalogNm.szStz(),
                                             sSchemaNm.szStz(),
                                             pTableCur->szTableNm(),
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
  }


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

  //------------------------------------------------------------------------
  // Init indexes table

//  Table* pTableIndexes = AM::pMetaDataTableIndexes();
//  StreamDef* pIndexesStreamDef = pTableIndexes->pStreamDef();
//  pTablePtn = pTableIndexes->pTablePtn(0);
//
//  pPdsRowBuffer = new Pds(pIndexesStreamDef, false, BT_DATA);
//  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
//                                  "rapids"_Stz,
//                                  "public"_Stz,
//                                  "dummy"_Stz,
//                                  "dummy"_Stz,
//                                  C_VARGS_END);
//
//  i8 nRows = 0;
//  pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
//  UNUSED(pRow);

  //------------------------------------------------------------------------
  // Init indexes table

//  Table* pTableIndexColumns = AM::pMetaDataTableIndexColumns();
//  StreamDef* pIndexColumnsStreamDef = pTableIndexColumns->pStreamDef();
//  pTablePtn = pTableIndexColumns->pTablePtn(0);
//
//  pPdsRowBuffer = new Pds(pIndexColumnsStreamDef, false, BT_DATA);
//  pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
//                                       "rapids"_Stz,
//                                       "public"_Stz,
//                                       "dummy"_Stz,
//                                       "dummy"_Stz,
//                                       "dummy"_Stz,
//                                       2,
//                                       C_VARGS_END);
//
//  nRows = 0;
//  pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
//  UNUSED(pRow);

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
