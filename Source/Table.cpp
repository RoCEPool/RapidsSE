/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "MetaData.h"


#include "Jobs.h"
#include "Partitioning.h"

Atomic gixTable;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(MetaData,Table,CN_TABLE)
__aNewFunction(Table,CN_TABLE)
__vDeleteFunction(Table,CN_TABLE)
__aFooterFunction(Table)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(Stz* szTableNm)
: Object(CN_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Table.m_nCreates++;
#endif
  
  m_sTableNm   = szTableNm;
  m_pIdxVec    = new IdxVec;
  m_ixIndexNo.vCheckInit();
  m_pBucketMap = new BucketMap(AM::nBuckets());
  m_pPtnMap    = new PtnMap(AM::nPartitions());
  m_bIsLoading = false;
  m_bDefinitionOnly = false;

  m_pStreamDef    = nullptr;
  m_pTablePtnVec  = new TablePtnVec;
  m_pPtnStreamDef = new StreamDef;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the table partitions
  for (i8 i_Slot = 0; i_Slot < AM::nSlotsPerNode(); i_Slot++) {
    
    TablePtn* pPartition = new TablePtn(this);
    m_pTablePtnVec->vAdd(pPartition);
    
    i8 iPartition = TablePtn::iSlotToPartition(i_Slot);
    m_pTablePtnVec->pGet(i_Slot)->vPartitionId(iPartition);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  _checkInitialised_(CN_TABLE);
  
  ++ gixTable;
  
  Job* pJob = AM::pThisTLSJob();
  
  AM::vLogDdl(SL_INF,pJob,DA_DDL_CREATE_TABLE,
              "%s |gixTable=%lld|"_Stz, //!format-LogOpr
              m_sTableNm.szStz(),* gixTable);

  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Table::~Table(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  m_pIdxVec->vClear();
  
  delete m_pIdxVec;
  delete m_pBucketMap;
  delete m_pPtnMap;

  delete m_pStreamDef;
  delete m_pPtnStreamDef;
  
  // delete the TablePtn's
  for (i8 i_Slot = 0; i_Slot < AM::nSlotsPerNode(); i_Slot++) {
    delete m_pTablePtnVec->pGet(i_Slot);
  }
  m_pTablePtnVec->vClear();
  
  // delete the partition vector
  delete m_pTablePtnVec;
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump table                                                                 */
/*----------------------------------------------------------------------------*/
void Table::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Table* pTable = _pTable(pObject);
  
  vTrace("name %s\n"_Stz,
         pTable->m_sTableNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* find table                                                                 */
/*----------------------------------------------------------------------------*/
Table* Table::pFindTable(DicStack* pDicStack, Stz* aTableNm)
{
  _static_
  
  Dic* pDic = pDicStack->pTop();
  
  Word* pTableDicWord = pDic->pFindWord(aTableNm);
  
  Dic* pTableDic = _pDic(pTableDicWord->pObject());
  
  Word* pTableWord = pTableDic->pFindWord(aTableNm);
  
  Table* pTable = _pTable(pTableWord->pObject());
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Table::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = AM::pStatistics();
  
  sPortrait.vSprint("Table: creates %lld drops %lld loads %lld\n"_Stz,
                    pStats->Table.m_nCreates,
                    pStats->Table.m_nDrops,
                    pStats->Table.m_nLoads
                    );
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index to table                                                         */
/*----------------------------------------------------------------------------*/
void Table::vAddIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // set the index no in the index
  pIndex->vIndexNo(++m_ixIndexNo);
  
  // add index to table
  m_pIdxVec->vAdd(pIndex);
  
  // create an IndexTree for the index in each partition
  for (i8 iPartition = 0; iPartition < AM::nSlotsPerNode(); iPartition++) {
    TablePtn* pPartition = this->pPartition(iPartition);
    pPartition->vAddIndex(new IndexTree(pIndex, pIndex->pIndexDef(), iPartition));
  }
}

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
void Table::vAnalyze(i8 iSlot, i8& nBuffers, i8& nRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nRowsTotal    = 0;
  i8 nBuffersTotal = 0;
  
  TablePtn* pPartition = this->pPartition(iSlot);
  
  CrwBuffer* pCrwBuffer = pPartition->pRootCrwBuffer();
  
  if (pCrwBuffer)
    pCrwBuffer->vAnalyze(nBuffers, nRows);
  
  nRowsTotal += nRows;
  nBuffersTotal += nBuffers;
}


/*----------------------------------------------------------------------------*/
/* create ptn stream def                                                      */
/*----------------------------------------------------------------------------*/
void Table::vCreatePtnDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Column* pColumn = nullptr;
  
  m_pPtnStreamDef->m_nDefSets = 1;
  m_pPtnStreamDef->m_i_DefStart[0] = 0;
  m_pPtnStreamDef->m_nDefColumns[0] = 0;
  
  // find the column from the table columnVector
  i8 iColumns = pStreamDef()->nColumns(C_ALL);
  
  for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
    pColumn = pStreamDef()->pGetColumn(i_Col);
    if (pColumn->bIsPtnKey()) {
      m_pPtnStreamDef->m_xaColumn[m_pPtnStreamDef->m_nDefColumns[0]].vCopy(pColumn);
      m_pPtnStreamDef->m_nDefColumns[0]++;
      m_pPtnStreamDef->m_nTotalColumns++;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get number of crw buffers in this table                                    */
/*----------------------------------------------------------------------------*/
i8 Table::nCrwBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
  for (i8 iPartition = 0; iPartition < AM::nSlotsPerNode(); iPartition++) {
    iBuffers += pPartition(iPartition)->nCrwBuffers();
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void Table::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vDump(CN_TABLE, this, Stz::szTab(1));
}

/*----------------------------------------------------------------------------*/
/* find column by name                                                        */
/*----------------------------------------------------------------------------*/
Column* Table::pFindColumn(Stz* szColNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Column* pColumn = nullptr;
  bool bFound = false;
  i8 nColumns = m_pStreamDef->nColumns(C_ALL);

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    pColumn = m_pStreamDef->pGetColumn(i_Col);
    if (pColumn->szColNm()->bSame(szColNm)) {
      bFound = true;
      break;
    }
  }
  
  return bFound ? pColumn : nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the index vector from the table                                        */
/*----------------------------------------------------------------------------*/
IdxVec* Table::pIdxVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pIdxVec;
}

/*----------------------------------------------------------------------------*/
/* get number of index buffers in this table                                  */
/*----------------------------------------------------------------------------*/
i8 Table::nIndexBuffers(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
  for (i8 iPartition = 0; iPartition < AM::nSlotsPerNode(); iPartition++) {
    IndexTree* pIndexTree = pPartition(iPartition)->pIndexTree(iIndexNo);
    
    iBuffers += pIndexTree->nEntBuffers();
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* insdel row buffer into the table indexes                                   */
/*----------------------------------------------------------------------------*/
void Table::vInsdelBuffer(i8 iTxId, i8 iSlot, CrwBuffer* pCrwBuffer)
{ //# indexTuning
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // This function is invoked when we insert rows into a table via a
  // begin/insert/end and via the API's insert-select
  
  ASSERT(iSlot < AM::nSlotsPerNode());
  
  // if buffer has 0 entries (only head) then should be last buffer
  if (pCrwBuffer->nRowCount() == 0) {
    delete pCrwBuffer;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark crwBuffer as permanent before its appended to the table
  
  pCrwBuffer->vIsPermanent(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append the buffer entries into the table
  
  TablePtn* pPartition = this->pPartition(iSlot);
  
  t8 tPerspectibe = AM::tThisTLSPespective();
  
  pPartition->vInsdelBuffer(pCrwBuffer,tPerspectibe);
}

/*----------------------------------------------------------------------------*/
/* insdel row buffer into the table indexes                                   */
/*----------------------------------------------------------------------------*/
void Table::vInsdelBuffer(i8 iTxId, i8 iSlot, RsbBuffer* pRsbBuffer)
{ //# indexTuning

  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // This function is invoked when we insert rows into a table via a
  // begin/insert/end and via the API's insert-select

  ASSERT(iSlot < AM::nSlotsPerNode());

  // if buffer has 0 entries (only head) then should be last buffer
  if (pRsbBuffer->nRsb() == 0) {
    delete pRsbBuffer;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark crwBuffer as permanent before its appended to the table

  pRsbBuffer->vIsPermanent(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append the buffer entries into the table

  TablePtn* pPartition = this->pPartition(iSlot);

  t8 tPerspective = 0; //AM::tThisTLSPespective();

  pPartition->vInsdelBuffer(pRsbBuffer,tPerspective);
}

/*----------------------------------------------------------------------------*/
/* get if table is a definition only                                          */
/*----------------------------------------------------------------------------*/
bool Table::bIsDefinitionOnly(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_bDefinitionOnly;
}

/*----------------------------------------------------------------------------*/
/* set if table if definition only                                            */
/*----------------------------------------------------------------------------*/
void Table::vIsDefinitionOnly(bool bDefinitionOnly)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_bDefinitionOnly = bDefinitionOnly;
}

/*----------------------------------------------------------------------------*/
/* get if table is loading                                                    */
/*----------------------------------------------------------------------------*/
bool Table::bIsLoading(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_bIsLoading;
}

/*----------------------------------------------------------------------------*/
/* set table is loading                                                       */
/*----------------------------------------------------------------------------*/
void Table::vIsLoading(bool bIsLoading)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_bIsLoading = bIsLoading;
}

/*----------------------------------------------------------------------------*/
/* lazy build of index                                                        */
/*----------------------------------------------------------------------------*/
void Table::vLazyBuildIndexes(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pPartition = this->pPartition(iSlot);
  IndexTreeVec* pInedexTreeVec = pPartition->pIndexTreeVec();
  i8 nIndexes = pInedexTreeVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pNewIndexTree = pInedexTreeVec->pGet(i_Index);
    
    Index* pIndex = pNewIndexTree->pIndex();
    
    // get an H0 buffer
    EntBuffer *pIndexBuffer = pPartition->pIndexTree(pIndex->iIndexNo())->pFirstH0Buffer();
    
    // build skip list for all H0 buffers
    if (pIndexBuffer)
      pIndexBuffer->vLazyBuild();
  }
}

/*----------------------------------------------------------------------------*/
/* mark table to activity map                                                 */
/*----------------------------------------------------------------------------*/
void Table::vMarkTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
 
  s48K sTableMark;
  
  sTableMark.vSprintCat("Table %s\n"_Stz,
                        m_sTableNm.szStz());
  
  i8 nCols = m_pStreamDef->nColumns(C_ALL);
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pColumn = m_pStreamDef->pGetColumn(i_Col);
    sTableMark.vSprintCat("  Col %02lld %s %s(%lld,%lld) %s\n"_Stz,
                          i_Col,
                          pColumn->szColNm(),
                          
                          aCT[pColumn->eColumnType()],
                          pColumn->iColumnLength(),
                          pColumn->iColumnScale(),
                          
                          pColumn->bIsNullable() ? "nullable" : "not-null",
                          pColumn->bIsPtnKey() ? "ptn-key" : ""
                          );

  }
  
  AM::vLogText(-1,sTableMark);
  
}

/*----------------------------------------------------------------------------*/
/* get a partition                                                            */
/*----------------------------------------------------------------------------*/
TablePtn* Table::pPartition(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pTablePtnVec->pGet(iSlot);;
}

/*----------------------------------------------------------------------------*/
/* populate the index                                                         */
/*----------------------------------------------------------------------------*/
bool Table::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 i_Partn = 0; i_Partn < AM::nSlotsPerNode(); i_Partn++) {
    bool bPopulated = m_pTablePtnVec->pGet(i_Partn)->bPopulateIndex(iIndexNo);
    
    if (! bPopulated)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove index from table                                                    */
/*----------------------------------------------------------------------------*/
void Table::vRemoveIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // remove index from the table
  m_pIdxVec->vRemove(pIndex);
  
  // remove the index from each tablePtn
  for (i8 i_Slot = 0; i_Slot < AM::nSlotsPerNode(); i_Slot++) {
    m_pTablePtnVec->pGet(i_Slot)->vRemoveIndex(pIndex->iIndexNo());
  }
}

/*----------------------------------------------------------------------------*/
/* get partition column vector                                                */
/*----------------------------------------------------------------------------*/
StreamDef*  Table::pPtnStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pPtnStreamDef;
}

/*----------------------------------------------------------------------------*/
/* publish rows/buffers                                                       */
/*----------------------------------------------------------------------------*/
void Table::vPublish(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vTrace("Publishing table %s\n"_Stz, m_sTableNm.azChar());
  
  // get the table ptn
  TablePtn* pTablePtn = m_pTablePtnVec->pGet(iPartition);
  
  // publish the ptn
  pTablePtn->vPublish();
  
  // publish all other indices
//  for (i8 iIndexNo = 0; iIndexNo < m_pIdxVec->nItems(); iIndexNo++) {
//    Index* pIndex = m_pIdxVec->pGet(iIndexNo);
//    m_xSlots.vPublishIndex(iPartition, pIndex->iIndexNo());
//  }
}

/*----------------------------------------------------------------------------*/
/* attempt to purge oldest buffer                                             */
/*----------------------------------------------------------------------------*/
bool Table::bPurge(i8 iPartition, i8 iOlderThanSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  //vTrace("Purging table %s\n"_Stz, m_sTableNm.azChar());
  
  // get the table ptn
  TablePtn* pTablePtn = m_pTablePtnVec->pGet(iPartition);
  
  // purge the table ptn
  bool bPurged = pTablePtn->bPurge(iOlderThanSeconds);
  
  // purge all other indexes
//  for (i8 iIndexNo = 0; iIndexNo < m_pIdxVec->nItems(); iIndexNo++) {
//
//    Index* pIndex = m_pIdxVec->pGet(iIndexNo);
//
//    bool bPurged = m_xSlots.bPurgeIndex(iPartition, pIndex->iIndexNo(), iOlderThanSeconds);
//
//    if (! bPurged)
//      return false;
//  }
  
  return bPurged;
}

/*----------------------------------------------------------------------------*/
/* get root buffer of given index                                             */
/*----------------------------------------------------------------------------*/
EntBuffer* Table::pRootBuffer(i8 i_Slot, i8 i_Index)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pTblPtn = pPartition(i_Slot);
  
  IndexTree* pIdxTree = pTblPtn->pIndexTree(i_Index);
  
  EntBuffer* pRootBuffer = pIdxTree->pRootBuffer();
  
  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get StreamDef                                                              */
/*----------------------------------------------------------------------------*/
StreamDef* Table::pStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* set StreamDef                                                              */
/*----------------------------------------------------------------------------*/
void Table::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
  
  for (i8 iPartition = 0; iPartition < AM::nSlotsPerNode(); iPartition++) {
    TablePtn* pPartition = this->pPartition(iPartition);
    ReadApi* pReadApi = pPartition->pReadApi();
    pReadApi->vStreamDef(pStreamDef);
  }
}

/*----------------------------------------------------------------------------*/
/* get the table name                                                         */
/*----------------------------------------------------------------------------*/
Stz* Table::szTableNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_sTableNm;
}

/*----------------------------------------------------------------------------*/
/* compute partition of given row hash                                        */
/*----------------------------------------------------------------------------*/
i8 Table::iToPartition(i8 iRowHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 i_Ptn = C_UNKNOWN;
  
#ifdef WITH_BUCKET_HASHING

  // modulo the hash to get bucket number
  i8 i_Bkt = iRowHash % C_MAX_BUCKETS;
  
  // lookup which partition a bucket number currently belongs to
  i_Ptn = m_pBucketMap->i_WhichPartition(i_Bkt);
  
#else
  
  // modulo the hash to get partition number
  i8 nPartitions = AM::nSlotsPerNode() * AM::nConnectedNodes();
  
  i_Ptn = iRowHash % nPartitions;

#endif
  
  return i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* upsert row buffer into the table indexes                                   */
/*----------------------------------------------------------------------------*/
void Table::vUpsertBuffer(i8 iTxId, i8 iSlot, CrwBuffer* pCrwBuffer)
{ //# indexTuning
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // This function is invoked when we insert rows into a table via a
  // begin/insert/end and via the API's insert-select
  
  ASSERT(iSlot < AM::nSlotsPerNode());
  
  // if buffer has 0 entries (only head) then should be last buffer
  if (pCrwBuffer->nRowCount() == 0) {
    delete pCrwBuffer;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark crwBuffer as permanent before its appended to the table
  
  pCrwBuffer->vIsPermanent(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append the buffer entries into the table
  
  TablePtn* pPartition = this->pPartition(iSlot);
  
  t8 tPerspectibe = AM::tThisTLSPespective();
  
  pPartition->vUpsertBuffer(pCrwBuffer,tPerspectibe);
}

/*----------------------------------------------------------------------------*/
/* verify all indexes                                                         */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pPartition = this->pPartition(0);             //# discuss with Alka
  IndexTreeVec* pIdxTreeVec = pPartition->pIndexTreeVec();
  i8 nIndexes = pIdxTreeVec->nItems();

  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    IndexTree* pIndexTree = pIdxTreeVec->pGet(i_Index);
    bool bValid = pIndexTree->bVerifyIndex();
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify index                                                               */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 iSlot = 0; iSlot < AM::nSlotsPerNode(); iSlot++) {
    
    bool bVerify = true;
    
    EntBuffer *pIndexBuffer =
      pPartition(iSlot)->pIndexTree(pIndex->iIndexNo())->pRootBuffer();
    
    if (pIndexBuffer)
      bVerify = pIndexBuffer->bVerify();
    if ( ! bVerify )
      break;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify table                                                               */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // verify all the row buffers with the r ow data
  for (i8 iSlot = 0; iSlot < AM::nSlotsPerNode(); iSlot++) {
    
    bool bVerify = true;
    
    bVerify = pPartition(iSlot)->bVerifyTable();
    
    if ( ! bVerify )
      return bVerify;
  }
  
  // now verify the indexes
  return bVerifyIndexes();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compact index                                                              */
/*----------------------------------------------------------------------------*/
void Table::vCompactIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 iSlot = 0; iSlot < AM::nSlotsPerNode(); iSlot++) {
    IndexTree *pIndexTree = pPartition(iSlot)->pIndexTree(pIndex->iIndexNo());
    pIndexTree->vCompact();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
