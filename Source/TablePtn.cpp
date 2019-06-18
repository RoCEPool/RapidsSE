/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
/*============================================================================*/

#include "Partitioning.h"
#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TablePtn* Partitioning::_pTablePtn(void* aAddr)
{
  TablePtn* pTablePtn = reinterpret_cast<TablePtn*>(aAddr);
  
  RASSERT_TYPE(pTablePtn,CN_TABLEPTN);
  
  return pTablePtn;
}

/*----------------------------------------------------------------------------*/
/* dump partition                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  TablePtn* pPartition = _pTablePtn(pObject);
  
  vTrace("m_iPartitionId %lld, DataVec %p\n"_Stz,
         pPartition->m_iPartitionId, pPartition->m_pIndexTreeVec);
}

/*----------------------------------------------------------------------------*/
/* convert partition number to node number                                    */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iPartitionToNode(i8 iPartition)
{
  i8 iNode = iPartition % gpAttachMap->m_pGrid->nNodes();
  
  return iNode;
}

/*----------------------------------------------------------------------------*/
/* convert partition number to slot number                                    */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iPartitionToSlot(i8 iPartition)
{
  i8 iSlot = iPartition / gpAttachMap->m_pGrid->nNodes();
  
  return iSlot;
}

/*----------------------------------------------------------------------------*/
/* convert slot number to partition number                                    */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iSlotToPartition(i8 iSlot)
{
  i8 iPartition =
   (iSlot * gpAttachMap->m_pGrid->nNodes()) + gpAttachMap->m_pGrid->iThisNode();
  
  return iPartition;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TablePtn::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TABLEPTN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TablePtn::operator delete(void* pBufferAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pBufferAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TablePtn::TablePtn(void)
: Object(CN_TABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_okLock.vInitLock(LK_PARTITION);
  m_iPartitionId = -1;
  m_pTable = nullptr;
  
  m_nSubLoadersDone  = 0;
  m_nSubInsertersDone = 0;
  m_bLowMemLoad = false;

  m_nWriteBehindBuffers = 0;
  m_nBuffersWritten     = 0;
  
  m_iParentJobId = C_UNKNOWN;
  
  m_pRootCrwBuffer  = nullptr;
  m_pCurrentCrwBuffer = nullptr;
  
  m_pIndexTreeVec = new IndexTreeVec;
  
  _checkInitialised_(CN_TABLEPTN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add no. of loadable buffers by a subloader into total for this partition   */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddLoadWriteBehindBuffers(i8 nBuffersFromSomeSubLoader)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(bIsLocked());
  ASSERT(C_UNKNOWN != nBuffersFromSomeSubLoader);
  
  m_nWriteBehindBuffers += nBuffersFromSomeSubLoader;
  m_nSubLoadersDone++;
}

/*----------------------------------------------------------------------------*/
/* add no. of insertable buffers by a subloader into total for this partition */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddInsertWriteBehindBuffers(i8 nBuffersFromSomeSubInserter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(bIsLocked());
  ASSERT(C_UNKNOWN != nBuffersFromSomeSubInserter);
  
  m_nWriteBehindBuffers += nBuffersFromSomeSubInserter;
  m_nSubInsertersDone++;
}

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_pIndexTreeVec->nItems() < C_MAX_INDEXES);
  m_pIndexTreeVec->vAdd(pIndexTree);
}

/*----------------------------------------------------------------------------*/
/* have all buffers been written?                                              */
/*----------------------------------------------------------------------------*/
bool TablePtn::bAllBuffersWritten(eDS eDataSource)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
//  vTrace("m_nSubLoadersDone %lld nSubLoadersInGrid %lld\n"
//         "m_nWriteBehindBuffers %lld m_nBuffersWritten %lld\n"_Stz,
//         m_nSubLoadersDone,
//         nSubLoadersInGrid,
//         m_nWriteBehindBuffers,
//         m_nBuffersWritten);
  
  bool bAllDone = false;
  
  if (eDataSource == DS_LOAD) {
    
    // compute the total number of subloaders in the grid
    i8 nSubLoadersInGrid = gpAttachMap->Arg.m_nSubLoaders;
    
    if (nSubLoadersInGrid) {
      
      if (m_nSubLoadersDone  == nSubLoadersInGrid &&
          m_nBuffersWritten  == m_nWriteBehindBuffers) {
        bAllDone = true;
      }
    }
    else {
      if (m_nBuffersWritten  == m_nWriteBehindBuffers) {
        bAllDone = true;
      }
    }
  }
 
  if (eDataSource == DS_INSERT) {
    
//    vTrace("SubInserters %lld SubInsertersDone %lld m_nBuffersWritten %lld m_nWriteBehindBuffers %lld\n"_Stz,
//           gpAttachMap->m_nSubInserters,
//           m_nSubInsertersDone,
//           m_nBuffersWritten,
//           m_nWriteBehindBuffers
//           );
    
    // compute the total number of subInserters in the grid
    i8 nSubInsertersInGrid = gpAttachMap->Arg.m_nSubInserters;
    
    if (nSubInsertersInGrid) {
      
      if (m_nSubInsertersDone  == nSubInsertersInGrid &&
          m_nBuffersWritten == m_nWriteBehindBuffers) {
        bAllDone = true;
      }
    }
    else {
      if (m_nBuffersWritten == m_nWriteBehindBuffers) {
        bAllDone = true;
      }
    }
  }
  
  if (bAllDone) {
    if (gpAttachMap->m_bVerbose)
      vTrace("All buffers have been loaded: %lld\n"_Stz,m_nBuffersWritten);
    
    // reset for next load
    m_nSubLoadersDone     = 0;
    m_nSubInsertersDone   = 0;
    m_nBuffersWritten     = 0;
    m_nWriteBehindBuffers = 0;
    m_iParentJobId        = -1;
    
    m_bLowMemLoad = false;
  }
  
  return bAllDone;
}

/*----------------------------------------------------------------------------*/
/* append a row to the row buffer into the TablePtn                           */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendRow(RowCompactor* pRowCompactor, Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nAppendBuffers++;
#endif
  
  RASSERT(m_okLock.bIsLocked());
  //m_okLock.vGrabWrite();
  
  if (m_pRootCrwBuffer == nullptr) {
    ASSERT( ! m_pCurrentCrwBuffer )
    m_pRootCrwBuffer = new CrwBuffer;
    m_pRootCrwBuffer->vTable(m_pTable);
    m_pRootCrwBuffer->vPartitionNo(m_iPartitionId);
    m_pCurrentCrwBuffer = m_pRootCrwBuffer;
  }
  
  bool bAppended = m_pCurrentCrwBuffer->bAppendRow(pRow);
  
  if ( ! bAppended ) {
    CrwBuffer* pCrwBuffer = new CrwBuffer;
    m_pRootCrwBuffer->vTable(m_pTable);
    m_pRootCrwBuffer->vPartitionNo(m_iPartitionId);
    pCrwBuffer->vNextBuffer(m_pCurrentCrwBuffer->pNextBuffer());
    pCrwBuffer->vPrevBuffer(m_pCurrentCrwBuffer);
    m_pCurrentCrwBuffer->vNextBuffer(pCrwBuffer);
    m_pCurrentCrwBuffer = pCrwBuffer;
    
    bAppended = m_pCurrentCrwBuffer->bAppendRow(pRow);
    RASSERT(bAppended);
  }
  
  // get the row from the buffer
  Id xId;
  i8 iRow = m_pCurrentCrwBuffer->nRowCount();
  xId.vItem(iRow);
  
  // get no of indexes of the table
  i8 nIndexes = m_pIndexTreeVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    
    // insert entry into other indexes
    bool bResult = pIndexTree->bInsertEntry(m_pCurrentCrwBuffer,iRow,xId);
    RASSERT(bResult);
  }
  
  //m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* append a row buffer into the TablePtn                                      */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nAppendBuffers++;
#endif
  
  RASSERT(m_okLock.bIsLocked());
  //m_okLock.vGrabWrite();

  if (m_pRootCrwBuffer == nullptr) {
    ASSERT( ! m_pCurrentCrwBuffer )
    m_pRootCrwBuffer = pCrwBuffer;
  }
  else {
    pCrwBuffer->vNextBuffer(m_pCurrentCrwBuffer->pNextBuffer());
    pCrwBuffer->vPrevBuffer(m_pCurrentCrwBuffer);
    m_pCurrentCrwBuffer->vNextBuffer(pCrwBuffer);
  }
  
  m_pCurrentCrwBuffer = pCrwBuffer;
  
  //m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* have all buffers been loaded?                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vWriteBehindBuffers(i8 nWriteBehindBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(bIsLocked());
  m_nWriteBehindBuffers = nWriteBehindBuffers;
}

/*----------------------------------------------------------------------------*/
/* have all buffers been loaded?                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vBuffersWritten(i8 nBuffersWritten)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(bIsLocked());
  m_nBuffersWritten = nBuffersWritten;
}

/*----------------------------------------------------------------------------*/
/* have all buffers been loaded?                                              */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nWriteBehindBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_nWriteBehindBuffers;
}

/*----------------------------------------------------------------------------*/
/* have all buffers been loaded?                                              */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuffersWritten(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_nBuffersWritten;
}

/*----------------------------------------------------------------------------*/
/* drop a read lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop a write lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
    
  while (pCrwBuffer) {
    pCrwBuffer->vShow();
    pCrwBuffer = pCrwBuffer->pNextBuffer();
  }

}

/*----------------------------------------------------------------------------*/
/* set endInsert jobid                                                        */
/*----------------------------------------------------------------------------*/
void TablePtn::vParentJobId(i8 iJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_iParentJobId = iJobId;
}

/*----------------------------------------------------------------------------*/
/* get endInsert jobid                                                        */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iParentJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_iParentJobId;
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a read lock on the partition                           */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  return m_okLock.bGrabRead();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a write lock on the partition                          */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* increment buffers written so far                                           */
/*----------------------------------------------------------------------------*/
void TablePtn::vIncBuffersWritten(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(bIsLocked());
  m_nBuffersWritten++;
}

/*----------------------------------------------------------------------------*/
/* get IndexTree                                                              */
/*----------------------------------------------------------------------------*/
IndexTree* TablePtn::pIndexTree(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = nullptr;
  i8 nIndexes = m_pIndexTreeVec->nItems();
  
  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    Index* pIndex = pIndexTree->pIndex();
    if (pIndex->iIndexNo() == iIndexNo)
      break;
  }
  
  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IndexTreeVec* TablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/* is the table partition locked?                                             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* get this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iPartitionId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_iPartitionId;
}

/*----------------------------------------------------------------------------*/
/* set this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
void TablePtn::vPartitionId(i8 iPartitionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_iPartitionId = iPartitionId;
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
  
  while (pCrwBuffer) {
    pIndexTree->vInsertEntriesFromBuffer(pCrwBuffer);
    pCrwBuffer = pCrwBuffer->pNextBuffer();
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* publish the index buffers                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vPublish(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  if ( ! m_pRootCrwBuffer)
    return;
  
  m_okLock.vGrabWrite();
  
  // is there a current buffer ?
  if ( ! m_pCurrentCrwBuffer) {
    m_okLock.vDropWrite();
    return;
  }
  
  ColumnVec* pColVec = m_pCurrentCrwBuffer->pTable()->pColumnVec();
  HV_Rec hvRec(pColVec);
  
  // create a row for retrieving the row
  HV_Row hRow(pColVec);
  
  // publish the current buffer (last buffer that was added)
  i8 nRowCount = m_pCurrentCrwBuffer->nRowCount();
  for (i8 i_Row = 0; i_Row < nRowCount; i_Row++) {
    
    // get the inflated compact row
    m_pCurrentCrwBuffer->vGetRow(hRow, i_Row);
    
    // append the row to the record
    hvRec->vAppend(hRow);
    
    vTrace("row: (%s)\n"_Stz,hvRec->szRecStr());
  }
  
  vTrace("Partition%lld: published %lld rows\n"_Stz,m_iPartitionId,
         m_pCurrentCrwBuffer->nRowCount());
  
  m_okLock.vDropWrite();
  
}

/*----------------------------------------------------------------------------*/
/* purge the table buffers                                                    */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPurge(i8 iOlderThanSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  static i8 iMaxAge = 0;
  static i8 nMaxBuffers = 0;
  
  if (! m_pRootCrwBuffer)
    return false;
  
  m_okLock.vGrabWrite();
  
  i8 iAge = m_pRootCrwBuffer->iAgeSsec();
  
  //  if (iAge <= 1) {
  //    vTrace("Purge: age is %llds\n"_Stz,iMaxAge);
  //  }
  
  if (iAge > iMaxAge) {
    iMaxAge = iAge;
    //    vTrace("Purge: max age now %llds\n"_Stz,iMaxAge);
  }
  
  i8 iLengthList = m_pRootCrwBuffer->iLengthList();
  if (iLengthList > nMaxBuffers) {
    nMaxBuffers = iLengthList;
    //vTrace("Purge: max buffer chain now %lld buffers\n"_Stz,nMaxBuffers);
  }
  
  
  if (iAge <= iOlderThanSeconds) {
    m_okLock.vDropWrite();
    return false;
  }
  
  // is currentBuffer same as rootBuffer ?
  if (m_pCurrentCrwBuffer == m_pRootCrwBuffer)
    m_pCurrentCrwBuffer = nullptr;
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
  
  // get next entry buffer
  m_pRootCrwBuffer = m_pRootCrwBuffer->pNextBuffer();
  
  // set prev pointer to null
  if (m_pRootCrwBuffer) {
    ASSERT(pCrwBuffer ==  m_pRootCrwBuffer->pPrevBuffer());
    m_pRootCrwBuffer->vPrevBuffer(nullptr);
  }
  
  m_okLock.vDropWrite();
  
  // delete the root buffer
  delete pCrwBuffer;

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nBuffersPurged++;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* reset the buffer write members                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vResetWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_nSubLoadersDone     = 0;
  m_nSubInsertersDone   = 0;
  m_nWriteBehindBuffers = 0;
  m_nBuffersWritten     = 0;
  m_iParentJobId        = -1;
  
  m_bLowMemLoad = false;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* TablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pRootCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the row buffers used                                                   */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nCrwBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  i8 nBufferCount = 0;
  
  if (m_pRootCrwBuffer)
    m_pRootCrwBuffer->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* TablePtn::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* tidy up                                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  // delete the index data vector
  delete m_pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/* tidy up                                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTidyUpIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
  // remove the index from the IdxDataVec
  m_pIndexTreeVec->vRemove(pIndexTree);
  delete pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  if (m_pRootCrwBuffer)
    m_pRootCrwBuffer->vTruncate();
  
  m_pRootCrwBuffer    = nullptr;
  m_pCurrentCrwBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool TablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
  bool bVerify = true;
  
  while (pCrwBuffer) {
    bVerify = pCrwBuffer->bVerify();
    if (! bVerify )
      break;
    pCrwBuffer = pCrwBuffer->pNextBuffer();
  }
  
  return bVerify;
}
/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void TablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
  bool bVerify = true;
  
  while (pCrwBuffer) {
    // print the decile of the this buffer
    vTrace("%lld"_Stz, pCrwBuffer->iDecile());
    pCrwBuffer = pCrwBuffer->pNextBuffer();
  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TablePtn::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
