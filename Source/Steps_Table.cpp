/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Table(void)
{
  vAddStep("table::delete"_Stz,            &Step::vTableDelete);
  vAddStep("table::new"_Stz,               &Step::vTableNew);
  
  vAddStep("table[]"_Stz,                  &Step::vTableGet);
  vAddStep("table[].set"_Stz,              &Step::vTableSet);
  
  vAddStep("table.addIndex"_Stz,           &Step::vTableAddIndex);
  vAddStep("table[].asAltStreamDef"_Stz,   &Step::vTableAsAltStreamDef);
  vAddStep("table[].asSrcStreamDef"_Stz,   &Step::vTableAsSrcStreamDef);
  vAddStep("table[].columnCount@"_Stz,     &Step::vTableColumnCountFetch);
  vAddStep("table[].column@"_Stz,          &Step::vTableColumnFetch);
  vAddStep("table.createPtnDef"_Stz,       &Step::vTableCreatePtnDef);
  vAddStep("table.deleteIndex"_Stz,        &Step::vTableDeleteIndex);
  vAddStep("table[].dump"_Stz,             &Step::vTableDump);
  vAddStep("table[].findColumn"_Stz,       &Step::vTableFindColumn);
  vAddStep("table[].genCsvBuffer"_Stz,     &Step::vTableGenCsvBuffer);
  vAddStep("table.indexCount"_Stz,         &Step::vTableIndexCount);
  vAddStep("table.isColumn"_Stz,           &Step::vTableIsColumn);
  vAddStep("table[].isDefinitionOnly@"_Stz, &Step::vTableIsDefinitionOnlyFetch);
  vAddStep("table[].isDefinitionOnly!"_Stz, &Step::vTableIsDefinitionOnlyStore);
  vAddStep("table.lazyBuildIndexes"_Stz,   &Step::vTableLazyBuildIndexes);
  vAddStep("table[].listPartition"_Stz,    &Step::vTableListPartition);
  vAddStep("table[].ptnStreamDef@"_Stz,    &Step::vTablePtnStreamDefFetch);
  vAddStep("table.purge"_Stz,              &Step::vTablePurge);
  vAddStep("table[].size"_Stz,             &Step::vTableSize);
  vAddStep("table[].streamDef@"_Stz,       &Step::vTableStreamDefFetch);
  vAddStep("table.streamDef!"_Stz,         &Step::vTableStreamDefStore);
  vAddStep("table[].tablePtn@"_Stz,        &Step::vTableTablePtnFetch);
  vAddStep("table[].tablePtnZero"_Stz,     &Step::vTableTablePtnZero);
  vAddStep("table[].verify"_Stz,           &Step::vTableVerify);
  vAddStep("table[].visualize"_Stz,        &Step::vTableVisualize);
}

/*----------------------------------------------------------------------------*/
/* table::new -- delete an instance of table                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableDelete(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  delete pTable;
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[] -- get table                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTableGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].set -- set table                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
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
/* table[].asAltStreamDef -- use this tables stream def as alt stream def     */
/*----------------------------------------------------------------------------*/
void Step::vTableAsAltStreamDef(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
    
  StreamDef* pStreamDef = pTable->pStreamDef();

  m_pLogicUnit->m_pDevAltRsb->m_pStreamDef = pStreamDef;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].asSrcStreamDef -- use this tables stream def as src stream def     */
/*----------------------------------------------------------------------------*/
void Step::vTableAsSrcStreamDef(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
    
  StreamDef* pStreamDef = pTable->pStreamDef();

  m_pLogicUnit->m_pDevSrcRsb->m_pStreamDef = pStreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.lazyBuildIndexes -- lazy build of indexes (from lazy chains)         */
/*----------------------------------------------------------------------------*/
void Step::vTableLazyBuildIndexes(void)
{
  ENTRY();
  i8 i_TablePtn;
  s48K sString;
  
  m_pDataStack->vPopStr(sString,__func__);
  
  Table* pTable = reinterpret_cast<Table*>(sString.iToI8());
  ASSERT(CN_TABLE == pTable->eClass());
  
  m_pDataStack->vPopStr(sString,__func__);
  
  i_TablePtn = sString.iToI8();
  
  pTable->vLazyBuildIndexes(i_TablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.createPtnDef -- create the table ptn def (stream def)                */
/*----------------------------------------------------------------------------*/
void Step::vTableCreatePtnDef(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vCreatePtnDef();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].column -- get the column from the table                            */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
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
  
  StreamDef* pStreamDef = m_pLogicUnit->m_xpTable->pStreamDef();
  i8 nColumns = pStreamDef->nColumns(C_ALL);
  
  m_pDataStack->vPushInt(nColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.deleteIndex -- remove an index from the table                   */
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
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::pGrid()->nTablePtns(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].genCsvBuffer -- generate a random row for table                    */
/*----------------------------------------------------------------------------*/
void Step::vTableGenCsvBuffer(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
  HV_Row hRow(pStreamDef,C_ALL);

  i8 nRecords = 0;
  for ( ; ; nRecords++) {
    
    hRow->vResetValues();
    RowGenerator::vRandom(hRow,pStreamDef);
    
    SP_Record spRecord(hRow);
    StrLib::vConcat(spRecord.sz()->azChar(),'\n');               //! sub-optimal
    //vTrace("Random Rec: %s"_Stz,spRecord.sz());

    if (! pCsvBuffer->szAppendRecord(spRecord.sz()))
      break;
  }
  
  //pCsvBuffer->vPrintRecords();
  
  //delete pCsvBuffer;
  //delete pRow;
  
  m_pDataStack->vPushPtr(pCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].streamDef@ -- fetch the table stream def                           */
/*----------------------------------------------------------------------------*/
void Step::vTableStreamDefFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].tablePtn@ -- fetch the table partition                              */
/*----------------------------------------------------------------------------*/
void Step::vTableTablePtnFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  Job* pJob = m_pTask->m_pJob;
  
  i8 i_Ptn = pJob->m_pOp->tPtnNo();
  i8 i_TablePtn = Table::iPtnNoToTablePtn(i_Ptn);
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
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  Job* pJob = m_pTask->m_pJob;
  
  tPN tPtnNo = 0;
  
  i8 i_TablePtn = Table::iPtnNoToTablePtn(tPtnNo);
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
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  s32 sColumnName;
  m_pDataStack->vPopStr(sColumnName,__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pColumn = nullptr;
  bool bFound = false;
  
  // look for the column in the streamDef
  i8 nTableColumns = pStreamDef->nColumns(C_ALL);
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
  i8 nTableColumns = pStreamDef->nColumns(C_ALL);
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
/* table.isDefinitionOnly -- is the table a definition only                   */
/*----------------------------------------------------------------------------*/
void Step::vTableIsDefinitionOnlyFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);

  m_pDataStack->vPushInt(pTable->bIsDefinitionOnly());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isDefinitionOnly! -- store table is definition only                  */
/*----------------------------------------------------------------------------*/
void Step::vTableIsDefinitionOnlyStore(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  pTable->vIsDefinitionOnly(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].listPartition -- list partition                                    */
/*----------------------------------------------------------------------------*/
void Step::vTableListPartition(void)
{
  ENTRY();
  
  Table* pStreamSetTable = _pTable(*m_pLogicUnit->m_xpStreamSetTable);

  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();

  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    "...tbd"_Stz,
                                    C_VARGS_END);
   
  //m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table::new -- create a new instance of table                               */
/*----------------------------------------------------------------------------*/
void Step::vTableNew(void)
{
  ENTRY();

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm,__func__);

  Table* pTable = new Table(sTableNm);
  
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].ptnStreamDef@ -- get the table ptn stream def                      */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnStreamDefFetch(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  StreamDef* pStreamDef = pTable->pPtnStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.purge -- purge rows/buffers in table on data stack                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePurge(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  i8 i_TablePtn = m_pDataStack->iPopAsInt(__func__);
  
  i8 iOlderThanSeconds = m_pDataStack->iPopAsInt(__func__);
  
  bool bPurged = pTable->bPurge(i_TablePtn,iOlderThanSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Table[].size -- push space used by table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSize(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  i8 iBuffers = pTable->nCrwBuffers();
  i8 iSize = iBuffers * C_SIZE_POOL_BUFFER;
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
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
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
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::pGrid()->nTablePtns(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    vTrace("Table Partition %lld\n"_Stz, i_TablePtn);
    pTablePtn->vVisualizeTable();
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
