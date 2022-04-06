/*============================================================================*/
/* Steps_Index.cpp - index step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "IndexDef.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Stage.hpp"
#include "Stream.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Index(void)
{
  vAddStep("index::delete"_Stz,         &Step::vIndexDelete);
  vAddStep("index::new"_Stz,            &Step::vIndexNew);
  
  vAddStep("Index[].columnCount@"_Stz,  &Step::vIndexColumnCountFetch);
  vAddStep("Index[].column@"_Stz,       &Step::vIndexColumnFetch);
  vAddStep("Index[].compact"_Stz,       &Step::vIndexCompact);
  vAddStep("Index[].compactPtn"_Stz,    &Step::vIndexCompactPtn);
  vAddStep("Index[].deleteNext"_Stz,    &Step::vIndexDeleteNext);
  vAddStep("Index[].dump"_Stz,          &Step::vIndexDump);
  vAddStep("index.indexDef@"_Stz,       &Step::vIndexIndexDefFetch);
  vAddStep("index.indexDef!"_Stz,       &Step::vIndexIndexDefStore);
  vAddStep("Index[].indexTree"_Stz,     &Step::vIndexIndexTree);
  vAddStep("index[].isNull"_Stz,        &Step::vIndexIsNull);
  vAddStep("index.isPrimaryKey@"_Stz,   &Step::vIndexIsPrimaryKeyFetch);
  vAddStep("index.isPrimaryKey!"_Stz,   &Step::vIndexIsPrimaryKeyStore);
  vAddStep("index.isUnique@"_Stz,       &Step::vIndexIsUniqueFetch);
  vAddStep("index.isValidUniqueIndex"_Stz,&Step::vIndexIsValidUniqueIndex);
  vAddStep("Index[].name@"_Stz,         &Step::vIndexNameFetch);
  vAddStep("index.populate"_Stz,        &Step::vIndexPopulate);
  vAddStep("Index[].size"_Stz,          &Step::vIndexSize);
  vAddStep("index.table@"_Stz,          &Step::vIndexTableFetch);
  vAddStep("index.table!"_Stz,          &Step::vIndexTableStore);
  vAddStep("Index[].verify"_Stz,        &Step::vIndexVerify);
  vAddStep("Index[].verifyPtn"_Stz,     &Step::vIndexVerifyPtn);
  vAddStep("Index[].visualize"_Stz,     &Step::vIndexVisualize);
  
  
  vAddStep("Index[]"_Stz,      &Step::vIndexGet);
  vAddStep("Index[].set"_Stz,  &Step::vIndexSet);

}

/*----------------------------------------------------------------------------*/
/* Index[].column@ -- get the column from the index                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  ASSERT(iColumnNo < pIndex->nColumns());
  Column* pColumn = pIndex->pGetColumn(iColumnNo);
  
  m_pDataStack->vPushPtr(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnCount@ -- fetch index column count                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnCountFetch(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  m_pDataStack->vPushInt(pIndex->nColumns());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].compact -- index compaction                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexCompact(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  pIndex->pTable()->vCompactIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].compactPtn -- compact index partition                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexCompactPtn(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIdxNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();

  PtnId tPtn  = m_pOp->tPtnId();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtn);
  
  IndexTree *pIndexTree = pTable->pTablePtn(i_TablePtn)->pIndexTree(iIdxNo);
  
  pIndexTree->vCompact();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::delete -- delete an instance of index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDelete(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

  delete pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].deleteNext -- delete rows within the range                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexDeleteNext(void)
{
  ENTRY();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index and the table
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table
  Table* pTable = pIndex->pTable();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the lower and upper entries
  Ent* aLowerEntry = m_pLogicUnit->m_aLowerEntry;
  Ent* aUpperEntry = m_pLogicUnit->m_aUpperEntry;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the entry buffer to start from
  EntBuffer* pEntBuffer = m_pLogicUnit->m_pEntBufferOuter;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get IndexTree
  i8 iIndexNo = pIndex->iIndexNo();
  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  HV_RowSingle hvRowSingle(pTable->pStreamDef());
  
  bool bBefore = true;
  Ent* aEntryToDelete;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get a lower entry ?
  if ( ! aLowerEntry) {
    goto L_Exit;
  }
  
  vTrace("------------------------------------------\n"_Stz);
  vTrace("Deleting rows:\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffers starting from aLowerEntry
  while (aLowerEntry) {
    
    // is lowerEntry before upperEntry ?
    if (aUpperEntry)
      bBefore = (aLowerEntry == aUpperEntry) ? false : true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lowerEntry is before upperEntry
    if (bBefore) {
      
      // get the crw entry from the lowerEntry
      Rrf xRef = aLowerEntry->rxRef();
        
      Crw* aCrw = CrwBuffer::pRefdCrw(xRef);
      
      RowInflator::vInflate_AllColumns_into_RowSingle(hvRowSingle,aCrw);
      hvRowSingle->vPrintRow();
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // delete the refernced crw
      bool bDeleted = CrwBuffer::bDeleteRefdCrw(xRef);
      ASSERT(bDeleted);
      UNUSED(bDeleted);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // save the entry
      aEntryToDelete = aLowerEntry;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      aLowerEntry = pEntBuffer->aNextEntry(aLowerEntry, 0);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // now purge the entry
      pIndexTree->vDelete(pEntBuffer, aEntryToDelete);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // no more entries ?
      if ( ! aLowerEntry ) {
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // move to next buffer
        pEntBuffer = pEntBuffer->pNextEntBuffer();
        
        // next buffer ?
        if ( pEntBuffer )
          aLowerEntry = pEntBuffer->aLeadEntry();
      }
    }
    else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // we are done
      aLowerEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    //if (pSession->m_bSessionExiting)
    //  aItem = nullptr;
  }
  
L_Exit:
  vTrace("------------------------------------------\n"_Stz);
  m_pLogicUnit->m_pEntBufferOuter = pEntBuffer;
  m_pLogicUnit->m_aLowerEntry = aLowerEntry;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].dump -- dump the index                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDump(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    
    pIndexTree->vDump(false);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.indexDef@ -- fetch index definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexIndexDefFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pIndex->pIndexDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.indexDef! -- store index definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexIndexDefStore(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  
  pIndex->vIndexDef(pIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].IndexTree -- get index tree of Index[] for current partition       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIndexTree(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  Table* pTable = pIndex->pTable();

  PtnId tPtnId;

  StreamDef* pStreamDef = pTable->pStreamDef();
  if (pStreamDef->bReplicated()) {
    
    Stage*  pStage   = m_pOp->pStage();
    Stream* pStream0 = pStage->pGet(0);
    Op*     pOp0_0   = pStream0->pGet(0);
    
    tPtnId = pOp0_0->tPtnId();
  }
  else {
    tPtnId = m_pOp->tPtnId();
  }
  
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnId);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

  i8 iIndexNo = pIndex->iIndexNo();

  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  m_pDataStack->vPushPtr(pIndexTree);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].isNull -- is index null ?                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpIndex.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey@ -- fetch isPrimaryKey from index                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsPrimaryKeyFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  bool bIsPrimaryKey = pIndex->bIsPrimaryKey();
  
  m_pDataStack->vPushInt(bIsPrimaryKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey! -- store isPrimaryKey into index                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsPrimaryKeyStore(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIsPrimaryKey = m_pDataStack->iPopInt(__func__);
  
  pIndex->vIsPrimaryKey(iIsPrimaryKey ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnique@ -- fetch isUnique from index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsUniqueFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  bool bIsUnique = pIndex->bIsUnique();
  
  m_pDataStack->vPushInt(bIsUnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isValidUniqueIndex -- is it a valid unqiue index                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsValidUniqueIndex(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  // get the table partition stream def
  PtnDef* pPtnDef = pIndex->pTable()->pPtnDef();
  
  // get the index column vector
  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxColumns = pIndexDef->nColumns();
  bool bValid = true;
  
  //! ALKA: shouldn't this be checking for the leading edge of the index to include
  //!  all partition keys ???
  // a unique index must include all columns in the partitioning keys
  i8 nPtnColumns = pPtnDef->nColumns();
  bool bFound = false;
  for (i8 i = 0; i < nPtnColumns; i++)
  {
    Column* pColumn = pPtnDef->pGetColumn(i);
    
    for (i8 i_Item = 0; i_Item < nIdxColumns; i_Item++) {
      if (pIndexDef->pGetColumn(i_Item)->iColumnNo() == pColumn->iColumnNo()) {
        bFound = true;
        break;
      }
    }
    
    if ( ! bFound ) {
      bValid = false;
      break;
    }
  }
  
  m_pDataStack->vPushInt(bValid ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.name@ -- fetch index name                                            */
/*----------------------------------------------------------------------------*/
void Step::vIndexNameFetch(void)
{
  ENTRY();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  s32 sIndexName = pIndex->szIndexNm();
  
  m_pDataStack->vPushStr(sIndexName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::new -- create a new instance of index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexNew(void)
{
  ENTRY();
  
  s32 sIndexNm;
  m_pDataStack->vPopStr(sIndexNm,__func__);

  Index* pIndex = new Index(sIndexNm);
  
  m_pDataStack->vPushPtr(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.populate -- populate the index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexPopulate(void)
{
  ENTRY();
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  bool bPopulated = pTable->bPopulateIndex(iIndexNo);
  
  m_pDataStack->vPushInt(bPopulated ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].size -- push space used by index                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexSize(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iBuffers = pIndex->pTable()->nIndexBuffers(pIndex->iIndexNo());
  i8 iSpace = iBuffers * C_SIZE_POOL_BUFFER;
  m_pDataStack->vPushInt(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table@ -- fetch table in index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTableFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  Table* pTable = pIndex->pTable();
  ASSERT(CN_TABLE == pTable->eGetClassNo());
  
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table! -- store table in index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTableStore(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TABLE == pTable->eGetClassNo());
  
  pIndex->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].verify -- verify index                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexVerify(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  bool bVerified = pIndex->pTable()->bVerifyIndex(pIndex);
  m_pDataStack->vPushInt(bVerified);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].verifyPtn -- verify partition of index                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexVerifyPtn(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIdxNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  PtnId tPtn  = m_pOp->tPtnId();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtn);

  IndexTree *pIndexTree = pTable->pTablePtn(i_TablePtn)->pIndexTree(iIdxNo);
  
  EntBuffer *pIndexRootBuffer = pIndexTree->pRootBuffer();

  bool bVerified;
  if (pIndexRootBuffer)
    bVerified = pIndexRootBuffer->bVerify();
  else
    bVerified = true;
  
  m_pDataStack->vPushInt(bVerified);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].visualize -- visualize the index                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexVisualize(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
    vTrace("Table Partition %lld\n"_Stz, i_TablePtn);
    pIndexTree->vVisualize();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[] -- get index                                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Index[].set -- set index                                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
