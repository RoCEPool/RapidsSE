/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "RowBuffer.hpp"

#include "StreamSet.hpp"
#include "PtnMap.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "MY.hpp"
#include "TagConstants.h"

#include <new>

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Table(void)
{
  vAddStep("table::delete"_Stz,            &Step::vTableDelete);
  vAddStep("table::new"_Stz,               &Step::vTableNew);
  
  vAddStep("Table[]"_Stz,                  &Step::vTableGet);
  vAddStep("Table[].set"_Stz,              &Step::vTableSet);
  
  vAddStep("table.addIndex"_Stz,           &Step::vTableAddIndex);
  vAddStep("Table[].asAltStreamDef"_Stz,   &Step::vTableAsAltStreamDef);
  vAddStep("Table[].asSrcStreamDef"_Stz,   &Step::vTableAsSrcStreamDef);
  vAddStep("Table[].columnCount@"_Stz,     &Step::vTableColumnCountFetch);
  vAddStep("Table[].column@"_Stz,          &Step::vTableColumnFetch);
  vAddStep("table.createPtnDef"_Stz,       &Step::vTableCreatePtnDef);
  vAddStep("table.deleteIndex"_Stz,        &Step::vTableDeleteIndex);
  vAddStep("Table[].dump"_Stz,             &Step::vTableDump);
  vAddStep("Table[].dumpmeta"_Stz,         &Step::vTableDumpMeta);
  vAddStep("Table[].findColumn"_Stz,       &Step::vTableFindColumn);
  vAddStep("table.indexCount"_Stz,         &Step::vTableIndexCount);
  vAddStep("table.isColumn"_Stz,           &Step::vTableIsColumn);
  vAddStep("table.getName"_Stz,            &Step::vTableGetName);
  vAddStep("Table[].isInternallyDefined"_Stz, &Step::vTableIsInternallyDefined);
  vAddStep("Table[].isUserCreated"_Stz,    &Step::vTableIsUserCreated);
  vAddStep("Table[].listPartition"_Stz,    &Step::vTableListPartition);
  vAddStep("Table[].name@"_Stz,            &Step::vTableNameFetch);
  vAddStep("Table[].shardDef@"_Stz,        &Step::vTableShardDefFetch);
  vAddStep("Table[].size"_Stz,             &Step::vTableSize);
  vAddStep("Table[].streamDef@"_Stz,       &Step::vTableStreamDefFetch);
  vAddStep("table.streamDef!"_Stz,         &Step::vTableStreamDefStore);
  vAddStep("Table[].tablePtn@"_Stz,        &Step::vTableTablePtnFetch);
  vAddStep("Table[].tablePtnGet"_Stz,      &Step::vTableTablePtnGet);
  vAddStep("Table[].tablePtnZero"_Stz,     &Step::vTableTablePtnZero);
  vAddStep("Table[].verify"_Stz,           &Step::vTableVerify);
  vAddStep("Table[].visualize"_Stz,        &Step::vTableVisualize);
}

/*----------------------------------------------------------------------------*/
/* table::delete -- delete an instance of table                               */
/*----------------------------------------------------------------------------*/
void Step::vTableDelete(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  //vTrace(">>>>>>>>>>>>Step::vTableDelete\n"_Stz);
  delete pTable;
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[] -- get table                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTableGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].set -- set table                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSet(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.addIndex -- add index to the table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableAddIndex(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  pTable->vAddIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].asAltStreamDef -- use this tables stream def as alt stream def     */
/*----------------------------------------------------------------------------*/
void Step::vTableAsAltStreamDef(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
    
  StreamDef* pStreamDef = pTable->pStreamDef();

  StreamDef* pStreamDefOld = _pStreamDef(m_p_TaskLogicUnit->m_pDevAltRow->m_pStreamDef);
  if (pStreamDefOld)
    delete pStreamDefOld;

  m_p_TaskLogicUnit->m_pDevAltRow->m_pStreamDef = new StreamDef(pStreamDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].asSrcStreamDef -- use this tables stream def as src stream def     */
/*----------------------------------------------------------------------------*/
void Step::vTableAsSrcStreamDef(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
    
  StreamDef* pStreamDef = pTable->pStreamDef();

  StreamDef* pStreamDefOld = _pStreamDef(m_p_TaskLogicUnit->m_pDevLftRow->m_pStreamDef);
  if (pStreamDefOld)
    delete pStreamDefOld;

  m_p_TaskLogicUnit->m_pDevLftRow->m_pStreamDef = new StreamDef(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.createPtnDef -- create the table ptn def (stream def)                */
/*----------------------------------------------------------------------------*/
void Step::vTableCreatePtnDef(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TABLE == pTable->eGetClassNo());
  
  pTable->vCreatePtnDef();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].column -- get the column from the table                            */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].columnCount -- get column count                                    */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnCountFetch(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = m_p_TaskLogicUnit->m_xpTable->pStreamDef();
  i8 nColumns = pStreamDef->nColumns();
  
  m_pDataStack->vPushInt(nColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.deleteIndex -- remove an index from the table                        */
/*----------------------------------------------------------------------------*/
void Step::vTableDeleteIndex(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

  pTable->vRemoveIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].dump                                                               */
/*----------------------------------------------------------------------------*/
void Step::vTableDump(void)
{
  ENTRY();
  
  vTrace("TableDump\n"_Stz);
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].dumpmeta                                                           */
/*----------------------------------------------------------------------------*/
void Step::vTableDumpMeta(void)
{
  ENTRY();

  s4K sDumpBuffer = "\n\n\n==== DumpMeta Table: ====================================\n"_Stz;

  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);


  sDumpBuffer.vCatSprint("Table:%s\n"_Stz, pTable->szTableNm()->azChar());

  i8 nCols = pTable->pStreamDef()->nColumns();
  for (i8 iCol = 0; iCol < nCols; iCol++) {
    Column* pCol = pTable->pStreamDef()->pGetColumn(iCol);
    sDumpBuffer.vCatSprint("    %s   %s"_Stz,    pCol->szColNm()->azChar(), aCT[pCol->eColumnType()]);

    if (pCol->iColumnLength()>0) {
      if (pCol->iColumnScale())
        sDumpBuffer.vCatSprint(" (%lld,%lld)"_Stz,pCol->iColumnLength(), pCol->iColumnScale());
      else
        sDumpBuffer.vCatSprint(" (%lld)"_Stz,pCol->iColumnLength());
    }
    if (pCol->bIsNullable())
      sDumpBuffer.vCatSprint(" nullable"_Stz);
    if (pCol->bIsUnique())
      sDumpBuffer.vCatSprint(" Unique"_Stz);
    if (pCol->bIsPtnKey())
      sDumpBuffer.vCatSprint(" Partition Key"_Stz);
    sDumpBuffer.vConcat("\n"_Stz);
  }

  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
#ifdef WITH_PTNID
    sDumpBuffer.vCatSprint("\nPartition %lld  %lld"_Stz, i_TablePtn, pTable->pTablePtn(i_TablePtn)->tPtnNo().iAsI8());
#else
    sDumpBuffer.vCatSprint("\nPartition %lld  %lld"_Stz, i_TablePtn, pTable->pTablePtn(i_TablePtn)->tPtnNo());
#endif
    //pTablePtn->vDump();
  }


  IdxVec* pIdxVec = pTable->pIdxVec();
  sDumpBuffer.vCatSprint("\n\nIndex: %lld\n"_Stz, pIdxVec->nItems());

  if (pIdxVec->nItems()>0) {
    for (i8 iI = 0; iI < pIdxVec->nItems(); iI++) {
      Index *pIdx = pIdxVec->pGet(iI);

      if (pIdx->eGetClassNo()==CN_INDEX && pIdx->nColumns() > 0) {
        Column* pCol = pIdx->pGetColumn(0);

        sDumpBuffer.vCatSprint("    %s  %s"_Stz, pIdx->szIndexNm()->azChar(), pCol->szColNm()->azChar());

        if (pCol->bIsUnique())
          sDumpBuffer.vCatSprint(" Unique"_Stz);
        if (pCol->bIsPtnKey())
          sDumpBuffer.vCatSprint(" Partition Key"_Stz);

      } else {
        sDumpBuffer.vCatSprint("  %s  "_Stz, aCN[pIdx->eGetClassNo()]);
      }
      sDumpBuffer.vConcat("\n"_Stz);
    }
  }

  ShardDef*  pShard = pTable->pShardDef();
  sDumpBuffer.vCatSprint("\n\nShard Name:%s\n"_Stz, pShard->szShardDefNm() ? pShard->szShardDefNm()->azChar() : "");
  for (i8 iC=0; iC<pShard->nColumns(); iC++) {
    sDumpBuffer.vCatSprint("    %s  "_Stz, pShard->pGetColumn(iC) ? pShard->pGetColumn(iC)->szColNm()->azChar() : "");
  }

  /*
  pIdxVec->nColumns();

      i8     nColumns(void);
  Column* pGetColumn(i8 iColumn);
  IndexDef* pIndexDef(void);
  void   vIndexDef(IndexDef* pIndexDef);
  Stz*   szIndexNm(void);
  */


  sDumpBuffer.vCatSprint("%s"_Stz,
                         "\n======================================================\n\n"_Stz);
  m_pDataStack->vPushStr(sDumpBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].streamDef@ -- fetch the table stream def                           */
/*----------------------------------------------------------------------------*/
void Step::vTableStreamDefFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].tablePtn@ -- fetch the table partition                             */
/*----------------------------------------------------------------------------*/
void Step::vTableTablePtnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  Op* pOp = MY::pOp();
#ifdef WITH_PTNID
  PtnId tPtnNo = pOp->tPtnNo();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnNo);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
#else
  tPN tPtnNo = pOp->tPtnNo();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnNo);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
#endif
  
  m_pDataStack->vPushPtr(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].tablePtnGet -- fetch given table partition[tPtnNo]                 */
/*----------------------------------------------------------------------------*/
void Step::vTableTablePtnGet(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
#ifdef WITH_PTNID
  PtnId tPtnId;
  tPtnId = m_pDataStack->iPopInt(__func__);
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnId);
#else
  tPN tPtnNo = m_pDataStack->iPopInt(__func__);
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnNo);
#endif

  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
  
  m_pDataStack->vPushPtr(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].tablePtnZer0 -- fetch table partition[0]                           */
/*----------------------------------------------------------------------------*/
void Step::vTableTablePtnZero(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);

#ifdef WITH_PTNID
  PtnId tPtnNo;
#else
  tPN tPtnNo = 0;
#endif
  
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnNo);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
  
  m_pDataStack->vPushPtr(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.indexCount -- get the no of indexes in the table                     */
/*----------------------------------------------------------------------------*/
void Step::vTableIndexCount(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pTable->pIdxVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.findColumn -- find the column from the table ?                       */
/*----------------------------------------------------------------------------*/
void Step::vTableFindColumn(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  s32 sColumnName;
  m_pDataStack->vPopStr(sColumnName,__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pColumn = nullptr;
  bool bFound = false;
  
  // look for the column in the streamDef
  i8 nTableColumns = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nTableColumns; i_Col++) {
    
    pColumn = pStreamDef->pGetColumn(i_Col);
    
    if (pColumn->szColNm()->bSame(sColumnName)) {
      bFound = true;
      break;
    }
  }
  RASSERT(bFound);
  
  m_pDataStack->vPushPtr(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isColumn -- is the column a valid column of the table ?              */
/*----------------------------------------------------------------------------*/
void Step::vTableIsColumn(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  s32 sColumnName;
  m_pDataStack->vPopStr(sColumnName,__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pColumn;
  bool bFound = false;
  
  // look for the column in the streamDef
  i8 nTableColumns = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nTableColumns; i_Col++) {
    
    pColumn = pStreamDef->pGetColumn(i_Col);
    
    if (pColumn->szColNm()->bSame(sColumnName)) {
      bFound = true;
      break;
    }
  }
  
  m_pDataStack->vPushInt(bFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isInternallyDefines -- is the table internally defined?              */
/*----------------------------------------------------------------------------*/
void Step::vTableIsInternallyDefined(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);

  m_pDataStack->vPushInt(pTable->bIsInternallyDefined());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isUserCreated -- is the table a user created table?                  */
/*----------------------------------------------------------------------------*/
void Step::vTableIsUserCreated(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);

  m_pDataStack->vPushInt(pTable->bIsUserCreated());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].listPartition -- list partition                                    */
/*----------------------------------------------------------------------------*/
void Step::vTableListPartition(void)
{
  ENTRY();
  
  s256 sOutput;
  
  Table* pStreamSetTable = _pTable(*m_p_TaskLogicUnit->m_xpStreamSetTable);

  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();

  StreamSet* pStreamSet = _pStreamSet(*m_p_TaskLogicUnit->m_xpStreamSet);
#ifdef WITH_PTNID
  PtnId tPtnId;
  tPtnId = m_pDataStack->iPopAsInt(__func__);

  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);

  for (;;) {
    i8 i_Node     = AM::pPtnMap()->iWhichNodeNo(tPtnId);
    i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnId);

    sOutput.vSprint("Ptn %lld : Node %lld TablePtn %lld"_Stz,
                    tPtnId.iAsI8(),i_Node,i_TablePtn);

    Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      sOutput.azChar(),
                                      C_VARGS_END);

    if (! pRow) {
      // call again
      m_pDataStack->vPushInt(tPtnId.iAsI8());
      break;
    }

    i8 iPtnId = tPtnId.iAsI8();
    if (++iPtnId >= AM::PTNMAP::nPtnsInGrid()) {
      // done
      m_pDataStack->vPushInt(false);
      break;
    }
  }
#else
  tPN tPtnNo = m_pDataStack->iPopAsInt(__func__);

  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);

  for (;;) {
    i8 i_Node     = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
    i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnNo);

    sOutput.vSprint("Ptn %lld : Node %lld TablePtn %lld"_Stz,
                    tPtnNo,i_Node,i_TablePtn);
  
    Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      sOutput.azChar(),
                                      C_VARGS_END);
  
    if (! pRow) {
      // call again
      m_pDataStack->vPushInt(tPtnNo);
      break;
    }
    
    if (++tPtnNo >= AM::PTNMAP::nPtnsInGrid()) {
      // done
      m_pDataStack->vPushInt(false);
      break;
    }
  }
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.getName -- fetch table name                                          */
/*----------------------------------------------------------------------------*/
void Step::vTableGetName(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  Stz* szTableNm = pTable->szTableNm();

  vTrace("Table name = %s \n"_Stz, szTableNm->azChar());

  m_pDataStack->vPushStz(szTableNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].name@ -- fetch table name                                          */
/*----------------------------------------------------------------------------*/
void Step::vTableNameFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  Stz* szTableNm = pTable->szTableNm();
  
  m_pDataStack->vPushStz(szTableNm);
  
  EXIT();
}
extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* table::new -- create a new instance of table                               */
/*----------------------------------------------------------------------------*/
void Step::vTableNew(void) {
  ENTRY();

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  Job *pJob = MY::pJob();
  TagSet *pJobTags = pJob->pJobTags();

  eTT eTableKind = TT_NORMAL;
  if (pJobTags && pJobTags->bHasName(C_TAG_INFOSET))
    eTableKind = TT_INFOSET;
  else if (pJobTags && pJobTags->bHasName(C_TAG_INTERNAL))
    eTableKind = TT_INTERNAL;
  else if (pJobTags && pJobTags->bHasName(C_TAG_REPLICATED))
    eTableKind = TT_REPLICATED;

  Table *pTable = new Table(sTableNm, eTableKind);


  // SAME NODE TESTING ////////////////////// SAME NODE TESTING //////////////////////////////
#ifdef JUNK
  if (sTableNm.bSame("bolTbl"_Stz)) {
    pTable->iPrepareParcel();

    s32 sTableName;

    Table *pTbl = nullptr;
    Table *pNewTable = nullptr;

    Parcel* pParcel = pTable->pParcel();
    i8 nObjParts = pParcel->nObjects();

    for (i8 i_Obj = 0; i_Obj < nObjParts; i_Obj++) {
      Object *pObject = pParcel->pGetObject(i_Obj);
      if (pObject->eGetClassNo() == CN_TABLE) {
        pTbl = (Table *) pObject;
        pNewTable = (Table *) ::operator new(gxaSizeHeap[pObject->eGetClassNo()].m_nClassSize, pTbl);

        pNewTable->iReassembleFromParcel(pTable->pParcel());
        vTrace(">>>>>>>>>>>> (New) Table:%s with %lld parts\n"_Stz, sTableName.azChar(), nObjParts);
        break;
      }
    }

    // Testing the concept, passing the minimum-reassemble table works
    m_pDataStack->vPushPtr(pNewTable);
  } else
  // END SAME NODE TESTING  //////////////////  END SAME NODE TESTING  /////////////////////////
#endif
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].shardDef@ -- get the table ptn stream def                          */
/*----------------------------------------------------------------------------*/
void Step::vTableShardDefFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  ASSERT(CN_TABLE == pTable->eGetClassNo());
  
  ShardDef* pShardDef = pTable->pShardDef();
  
  m_pDataStack->vPushPtr(pShardDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].size -- push space used by table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSize(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  i8 nBuffers = pTable->nBuffers();
  i8 iSize = nBuffers * C_SIZE_POOL_BUFFER;
  m_pDataStack->vPushInt(iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.streamDef! -- store stream definition                                */
/*----------------------------------------------------------------------------*/
void Step::vTableStreamDefStore(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  pTable->vStreamDef(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].verify -- verify the table                                         */
/*----------------------------------------------------------------------------*/
void Step::vTableVerify(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  bool bVerified = pTable->bVerifyTable();
  
  m_pDataStack->vPushInt(bVerified);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].visualize -- visualize the table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableVisualize(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_p_TaskLogicUnit->m_xpTable);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Table Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vVisualizeTable();
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
