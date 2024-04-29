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
  vAddStep("MetaDataTable[].catalog"_Stz,       &Step::vMetaDataTableCatalog);
  vAddStep("MetaDataTable[].cluster"_Stz,       &Step::vMetaDataTableCluster);
  vAddStep("MetaDataTable[].columns"_Stz,       &Step::vMetaDataTableColumns);
  vAddStep("MetaDataTable[].indexColumns"_Stz,  &Step::vMetaDataTableIndexColumns);
  vAddStep("MetaDataTable[].indexes"_Stz,       &Step::vMetaDataTableIndexes);
  vAddStep("MetaDataTable[].nodes"_Stz,         &Step::vMetaDataTableNodes);
  vAddStep("MetaDataTable[].queries"_Stz,       &Step::vMetaDataTableQueries);
  vAddStep("MetaDataTable[].schema"_Stz,        &Step::vMetaDataTableSchema);
  vAddStep("MetaDataTable[].sessions"_Stz,      &Step::vMetaDataTableSessions);
  vAddStep("MetaDataTable[].tables"_Stz,        &Step::vMetaDataTableTables);


  vAddStep("MetaDataTable[].insertTables"_Stz,  &Step::vMetaDataTableTablesInsert);
  vAddStep("MetaDataTable[].setAM"_Stz,  &Step::vMetaDataTableSetAM);
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].catalog --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCatalog(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

  Word* pCatalog = m_oDicStack.pFindWord("catalogs"_Stz);
  Dic* pCatalogDic = _pDic(pCatalog->pObject());

  // Get last word in dic catalog
  Word* pCatalogFolderWord = pCatalogDic->pLastWord();

  // iterate dic catalog
  while(pCatalogFolderWord) {

    // get catalog name
    s256 sCatalogNm(pCatalogFolderWord->szWordNm());

    Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         sCatalogNm.szStz(),
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

//    vTrace("%s\n"_Stz, sCatalogNm.szStz());

    pCatalogFolderWord = pCatalogFolderWord->pPrevWord();
  }

  if(pPdsRowBuffer->nItems() > 0) {
    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].cluster --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCluster(void)
{
  ENTRY();


  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                       gxCmdLineArgs.m_xSysArgs.m_iClusterID,
                                       C_VARGS_END);
  if (! pRow)
  {
    ASSERT(0);
  }

  // Pipe out this buffer
  BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
  AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
  Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
  AM::OP::vPipeToNextOp(m_pOp,pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].columns --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableColumns(void)
{
  ENTRY();
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

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

    pTableFolderWord = pTableFolderWord->pPrevWord();
  }

  if(pPdsRowBuffer->nItems() > 0) {
    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].indexColumns --                                            */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableIndexColumns(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

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

    // find indexes folder
    Word* pIndexesFolder = pDicTable->pFindWord("indexes"_Stz);

    // Iterate the indexes folder if it exists
    if(pIndexesFolder) {
      Dic* pIndexesDic = _pDic(pIndexesFolder->pObject());
      Word* pIndexWord = pIndexesDic->pLastWord();

      while(pIndexWord) {
        Index* pIndex = _pIndex(pIndexWord->pObject());

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
                                               C_VARGS_END);
          i_Col++;

          if (!pRow) {
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
        pIndexWord = pIndexWord->pPrevWord();
      }
    }
    pTableFolderWord = pTableFolderWord->pPrevWord();
  }

  if(pPdsRowBuffer->nItems() > 0) {
    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].indexes --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableIndexes(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

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

    // find indexes folder
    Word* pIndexesFolder = pDicTable->pFindWord("indexes"_Stz);

    // Iterate the indexes folder if it exists
    if(pIndexesFolder) {
      Dic* pIndexesDic = _pDic(pIndexesFolder->pObject());
      Word* pIndexWord = pIndexesDic->pLastWord();

      while(pIndexWord) {
        Index* pIndex = _pIndex(pIndexWord->pObject());

        // Add index info to pds
        Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    sCatalogNm.szStz(),
                                    sSchemaNm.szStz(),
                                    pTableCur->szTableNm(),
                                    pIndex->szIndexNm(),
                                    C_VARGS_END);

        if(!pRow) {
          // Pipe out this full buffer
          BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
          AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
          Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
          AM::OP::vPipeToNextOp(m_pOp,pOperand,this);

          // create a new buffer and continue adding rows to it
          pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);
          break;
        }
        pIndexWord = pIndexWord->pPrevWord();
      }
    }

//    vTrace("%s\n"_Stz, pTableCur->szTableNm());

    pTableFolderWord = pTableFolderWord->pPrevWord();
  }

  if(pPdsRowBuffer->nItems() > 0) {
    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].nodes --                                                   */
/*----------------------------------------------------------------------------*/
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
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].queries --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableQueries(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].schema --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSchema(void)
{
  ENTRY();
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

  // get current folder
  i8 iDepth =  m_oDicStack.nItems();
  Object* pPtrDic =  m_oDicStack.pPick(iDepth - 1);

  // Get table dict
  Dic* pDic = _pDic(pPtrDic);

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

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                       sCatalogNm.szStz(),
                                       sSchemaNm.szStz(),
                                       C_VARGS_END);

  if(! pRow) {
    ASSERT(0);
  }

  if(pPdsRowBuffer->nItems() > 0) {
    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].sessions --                                                */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSessions(void)
{
  ENTRY();

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* MetaDataTable[].tables --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableTables(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

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

    Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         sCatalogNm.szStz(),
                                         sSchemaNm.szStz(),
                                         pTableCur->szTableNm(),
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
    pTableFolderWord = pTableFolderWord->pPrevWord();
  }

  if(pPdsRowBuffer->nItems() > 0) {
    BufferId oBufferId = pPdsRowBuffer->oGetBufferId().m_pvAsAddress;
    AM::pHashMap()->vDropKeyLock(oBufferId.m_pvAsAddress);
    Operand* pOperand = _pOperand((Object*)oBufferId.m_pvAsAddress);
    AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  } else
    delete(pPdsRowBuffer);

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
  m_pDataStack->vPopStr(szTableNm,__func__);

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
    SkipListBlock* pHead =pPdsRootBuffer->pFindLessThan((void*) &iSearchKey);
    if(pHead != nullptr) {
      // If the block is not null, extract the keys and values
      PdsBufferIdItem* pBufferIdStruct = (PdsBufferIdItem*) pHead->aItem();

      HashMapEntry* pEntry = AM::pHashMap()->pEntryAt(pBufferIdStruct->oBufferId.m_pvAsAddress);

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
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].setAM --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSetAM(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__ ));

  if(pTable->bIsMetaDataTable())
    AM::ATTACHMAP::vMetaDataTables(pTable);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
