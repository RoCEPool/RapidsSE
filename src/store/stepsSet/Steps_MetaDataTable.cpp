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

extern CmdLineArgs  gxCmdLineArgs;
extern AttachMap*   gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaDataTable(void)
{
  vAddStep("MetaDataTable[].select"_Stz,        &Step::vMetaDataTableSelect);


  vAddStep("MetaDataTable[].insertTables"_Stz,  &Step::vMetaDataTableTablesInsert);
  vAddStep("MetaDataTable[].deleteTables"_Stz,  &Step::vMetaDataTableTablesDelete);
  vAddStep("MetaDataTable[].insertCatalog"_Stz,  &Step::vMetaDataTableCatalogInsert);
  vAddStep("MetaDataTable[].deleteCatalog"_Stz,  &Step::vMetaDataTableCatalogDelete);
//  vAddStep("MetaDataTable[].insertCluster"_Stz,  &Step::vMetaDataTableClusterInsert);
  vAddStep("MetaDataTable[].insertColumns"_Stz,  &Step::vMetaDataTableColumnsInsert);
//  vAddStep("MetaDataTable[].insertIndexColumns"_Stz,  &Step::vMetaDataTableIndexColumnsInsert);
//  vAddStep("MetaDataTable[].insertIndexes"_Stz,  &Step::vMetaDataTableIndexInsert);
//  vAddStep("MetaDataTable[].insertNodes"_Stz,  &Step::vMetaDataTableNodesInsert);
//  vAddStep("MetaDataTable[].insertQueries"_Stz,  &Step::vMetaDataTableQueriesInsert);
//  vAddStep("MetaDataTable[].insertSessions"_Stz,  &Step::vMetaDataTableSessionsInsert);
  vAddStep("MetaDataTable[].insertSchema"_Stz,  &Step::vMetaDataTableSchemaInsert);

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
/* MetaDataTable[].nodes --                                                   */
/*----------------------------------------------------------------------------*/
/*
void Step::vMetaDataTableNodes(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);


  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         (*oNode_I)->iNodeId(),
                                         (*oNode_I)->szIPAddr(),
                                         (*oNode_I)->iPortNo(),
                                         0,
                                         C_VARGS_END);
    if(! pRow) {
      // Pipe out this full buffer
      BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
      AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
      Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
      AM::OP::vPipeToNextOp(m_pOp,pOperand,this);

      // create a new buffer and continue adding rows to it
      pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
      break;
    }
  }

  if(pPdsRowBuffer->nItems() > 0) {
    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
    vTrace("%p %lld\n"_Stz, oBufferId.m_pvAsAddress, AM::pHashMap()->bIsLocked(oBufferId.m_pvAsAddress));
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

  EXIT();
}
*/
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
/* MetaDataTable[].deleteTables --                                            */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableTablesDelete(void)
{
  ENTRY();

  s256 szTableNm;
  m_pDataStack->vPopStr(szTableNm,__func__);

  Table* pTable = AM::pMetaDataTableOfTables();
  if(!pTable) {
    EXIT();
    return;
  }
  TablePtn* pTablePtn = pTable->pTablePtn(0);

  BufferId oRootBufferId = pTablePtn->oRootBufferId();
  Pds* pRootPds = _pPds(AM::pHashMap()->pValueAt(oRootBufferId.m_pvAsAddress));

  PdsCursor* pCursor = new PdsCursor(pRootPds);

  while(! pCursor->bIsCurrNull()) {
    SkipListBlock* pSLBlock = pCursor->m_pCurrentBlock;

    BufferId oBufferId = pTablePtn->oExtractBufferId((Object*)pSLBlock->aItem());
    Pds* pPdsRowBuffer = _pPds(AM::pHashMap()->pValueAt(oBufferId.m_pvAsAddress, true));

    PdsCursor* pRowCursor = new PdsCursor(pPdsRowBuffer);

    while(! pRowCursor->bIsCurrNull()) {
      Row* pRow = pRowCursor->pCurrRow();

      Stz* szRowTableNm = pRow->szColVal_ofs(2);

      if(szRowTableNm->bSame(szTableNm)) {
        i8* pKey = (i8*)pRowCursor->pCurrKey();

        bool bResult = pPdsRowBuffer->bDelete(pKey);
        ASSERT(bResult);
        UNUSED(bResult);

        delete pRowCursor;
        delete pCursor;

        AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
        AM::pHashMap()->vDropKeyLock(oRootBufferId.m_pvAsAddress);
        EXIT();
        return;
      }
      pRowCursor->vNext();
    }
    delete pRowCursor;
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
    pCursor->vNext();
  }
  delete pCursor;
  AM::pHashMap()->vDropKeyLock(oRootBufferId.m_pvAsAddress);
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
/* MetaDataTable[].insertColumns --                                           */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableColumnsInsert(void)
{
  ENTRY();
  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  Table* pColumnsTable = AM::pMetaDataTableColumns();
  if(!pColumnsTable) {
    EXIT();
    return;
  }

  StreamDef* pColumnsStreamDef = pColumnsTable->pStreamDef();
  TablePtn* pTablePtn = pColumnsTable->pTablePtn(0);
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


  // get current folder
  i8 iDepth =  m_oDicStack.nItems();
  Object* pPtrDic =  m_oDicStack.pPick(iDepth - 1);

  // Get table dict
  Dic* pDic = _pDic(pPtrDic);
  Word* pTableFolderWord = pDic->pLastWord();

  // get the schema name
  i8 i = 2;
  while( pDic->eDicKind() != DK_SCHEMA) {
    pPtrDic = m_oDicStack.pPick(iDepth - i);
    pDic = _pDic(pPtrDic);
    i++;
  }

  s256 sSchemaNm(pDic->szDicNm());

  // get the catalog name
  while( pDic->eDicKind() != DK_CATALOG) {
    pPtrDic = m_oDicStack.pPick(iDepth - i);
    pDic = _pDic(pPtrDic);
    i++;
  }

  s256 sCatalogNm(pDic->szDicNm());

  // iterate dic table

  while(pTableFolderWord) {
    // get table name
    s256 sTableNm(pTableFolderWord->szWordNm());

    // find tableNm folder in current dic
    Word *pWordTable = m_oDicStack.pFindWord(sTableNm);
    Dic *pDicTable = _pDic(pWordTable->pObject());

    // find actual table in this table dic
    Word *pWord = pDicTable->pFindWord(sTableNm);
    Table *pTableCur = _pTable(pWord->pObject());

    if( ! pTableCur->bIsUserCreated()) {
      pTableFolderWord = pTableFolderWord->pPrevWord();
      continue;
    }

    StreamDef* pStreamDef = pTableCur->pStreamDef();

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
      i_Col++;

      if (!pRow) {
        ASSERT(0);
        break;
      }
    }

    pTableFolderWord = pTableFolderWord->pPrevWord();
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

  s256 szCatalogNm;
  m_pDataStack->vPopStr(szCatalogNm,__func__);

  s256 szSchemaNm;
  m_pDataStack->vPopStr(szSchemaNm,__func__);

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
/* MetaDataTable[].deleteCatalog --                                           */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCatalogDelete(void)
{
  ENTRY();

  s256 szCatalogNm;
  m_pDataStack->vPopStr(szCatalogNm,__func__);

  Table* pTable = AM::pMetaDataTableOfTables();
  if(!pTable) {
    EXIT();
    return;
  }
  TablePtn* pTablePtn = pTable->pTablePtn(0);

  BufferId oRootBufferId = pTablePtn->oRootBufferId();
  Pds* pRootPds = _pPds(AM::pHashMap()->pValueAt(oRootBufferId.m_pvAsAddress));

  PdsCursor* pCursor = new PdsCursor(pRootPds);

  while(! pCursor->bIsCurrNull()) {
    SkipListBlock* pSLBlock = pCursor->m_pCurrentBlock;

    BufferId oBufferId = pTablePtn->oExtractBufferId((Object*)pSLBlock->aItem());
    Pds* pPdsRowBuffer = _pPds(AM::pHashMap()->pValueAt(oBufferId.m_pvAsAddress, true));

    PdsCursor* pRowCursor = new PdsCursor(pPdsRowBuffer);

    while(! pRowCursor->bIsCurrNull()) {
      Row* pRow = pRowCursor->pCurrRow();

      Stz* szRowTableNm = pRow->szColVal_ofs(0);

      if(szRowTableNm->bSame(szCatalogNm)) {
        i8* pKey = (i8*)pRowCursor->pCurrKey();

        bool bResult = pPdsRowBuffer->bDelete(pKey);
        ASSERT(bResult);
        UNUSED(bResult);

        delete pRowCursor;
        delete pCursor;

        AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
        AM::pHashMap()->vDropKeyLock(oRootBufferId.m_pvAsAddress);
        EXIT();
        return;
      }
      pRowCursor->vNext();
    }
    delete pRowCursor;
    AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
    pCursor->vNext();
  }
  delete pCursor;
  AM::pHashMap()->vDropKeyLock(oRootBufferId.m_pvAsAddress);
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
      i_Col++;

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
      }
    }

    if(bAppend) {
      i8 nRows = 0;
      pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
    } else {
      AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
    }


    //------------------------------------------------------------------------
    // Insert into indexes

    Table* pTableIndexes = AM::pMetaDataTableIndexes();
    StreamDef* pIndexesStreamDef = pTableIndexes->pStreamDef();
    pTablePtn = pTableIndexes->pTablePtn(0);
    pPdsRootBuffer = _pPds(
        AM::pHashMap()->pValueAt(
            pTablePtn->oRootBufferId().m_pvAsAddress, true
        ));

    pPdsRowBuffer = nullptr;
    bAppend = false;
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

    IdxVec* pIndexVector = pTableCur->pIdxVec();
    i8 iNumIndexes = pIndexVector->nItems();
    for(i8 i_idx = 0; i_idx < iNumIndexes; i_idx++) {
      Index* pIndex = pIndexVector->pGet(i_idx);

      // Add index info to pds
      Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                           sCatalogNm.szStz(),
                                           sSchemaNm.szStz(),
                                           pTableCur->szTableNm(),
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
      }
    }

    if(bAppend) {
      i8 nRows = 0;
      pTablePtn->vAppendBuffer(pPdsRowBuffer, nRows);
    } else {
      AM::pHashMap()->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
    }

    //------------------------------------------------------------------------
    // Insert into indexColumns

    Table* pTableIndexCols = AM::pMetaDataTableIndexes();
    StreamDef* pIndexColsStreamDef = pTableIndexes->pStreamDef();
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

    for(i8 i_idx = 0; i_idx < iNumIndexes; i_idx++) {
      Index* pIndex = pIndexVector->pGet(i_idx);

      i8 nColumns = pIndex->nColumns();
      i8 i_Col = 0;

      while(i_Col < nColumns) {
        // Add index info to pds
        Row *pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                             sCatalogNm.szStz(),
                                             sSchemaNm.szStz(),
                                             pTableCur->szTableNm(),
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
        }
      }
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
