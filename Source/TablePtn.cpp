/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
/*============================================================================*/

#include "MetaData.h"
#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TablePtn* MetaData::_pTablePtn(void* aAddr)
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
  
  m_nSubLoadersDone  = 0;
  m_nSubInsertersDone = 0;
  m_bLowMemLoad = false;

  m_nWriteBehindBuffers = 0;
  m_nBuffersWritten     = 0;
  
  m_iParentJobId = C_UNKNOWN;
  
//#ifdef WITH_ROWBUFFER
  m_pRootRowBuffer  = nullptr;
  m_pCurrentRowBuffer = nullptr;
//#endif
  
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
  
  ASSERT(m_pIndexTreeVec->nItems() < C_INDEXES_PER_TABLE);
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
/* append a row buffer into the TablePtn                                      */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  gpAttachMap->m_oStatistics.IndexTree.m_nAppendBuffers++;
  
  RASSERT(m_okLock.bIsLocked());
  //m_okLock.vGrabWrite();
#ifdef WITH_ROWBUFFER
  if (m_pRootRowBuffer == nullptr) {
    ASSERT( ! m_pCurrentRowBuffer )
    m_pRootRowBuffer = pRowBuffer;
  }
  else {
    pRowBuffer->vNextBuffer(m_pCurrentRowBuffer->pNextBuffer());
    pRowBuffer->vPrevBuffer(m_pCurrentRowBuffer);
    m_pCurrentRowBuffer->vNextBuffer(pRowBuffer);
  }
  
  m_pCurrentRowBuffer = pRowBuffer;
#endif
  
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
  
  RowBuffer* pRowBuffer = m_pRootRowBuffer;
    
  while (pRowBuffer) {
    pRowBuffer->vShow();
    pRowBuffer = pRowBuffer->pNextBuffer();
  }

}

/*----------------------------------------------------------------------------*/
/* get the first row buffer                                                   */
/*----------------------------------------------------------------------------*/
RowBuffer* TablePtn::pFirstRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pRootRowBuffer;
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
/* get hidden IndexTree                                                       */
/*----------------------------------------------------------------------------*/
IndexTree* TablePtn::pHiddenIndexTree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = nullptr;
  pIndexTree = m_pIndexTreeVec->pGet(0);
  
  return pIndexTree;
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
  
  IndexTree* pHiddenIndexTree = this->pHiddenIndexTree();
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
#ifdef WITH_ROWBUFFER
  RowBuffer* pRowBuffer = pHiddenIndexTree->pFirstHiddenBuffer();
  
  while (pRowBuffer) {
    
    //    while (aItem) {
    //
    //      // insert entry
    //      if ( ! pIndexTree->bInsertEntry(aItem))
    //        return false;
    //
    //      // get next entry
    //      aItem = pEntryBuffer->pNextEntry(aItem, 0);
    //    }
    
    
    pIndexTree->vInsertEntriesFromBuffer(pRowBuffer);
    
    pIndexTree->vInsertEntriesFromBuffer(pRowBuffer);
    pRowBuffer = pRowBuffer->pNextBuffer();
  }
#else
  
  EntryBuffer* pEntryBuffer = pHiddenIndexTree->pFirstH0Buffer();
  
  while (pEntryBuffer) {
    
//    while (aItem) {
//
//      // insert entry
//      if ( ! pIndexTree->bInsertEntry(aItem))
//        return false;
//
//      // get next entry
//      aItem = pEntryBuffer->pNextEntry(aItem, 0);
//    }


    pIndexTree->vInsertEntriesFromBuffer(pEntryBuffer);
    pEntryBuffer = pEntryBuffer->pNextBuffer();
  }
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
RowBuffer* TablePtn::pRootRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pRootRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the row buffers used                                                   */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  i8 nBufferCount = 0;
  
  if (m_pRootRowBuffer)
    m_pRootRowBuffer->vBufferCount(nBufferCount);
  
  return nBufferCount;
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
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool TablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  RowBuffer* pRowBuffer = m_pRootRowBuffer;
  bool bVerify = true;
  
  while (pRowBuffer) {
    bVerify = pRowBuffer->bVerify();
    if (! bVerify )
      break;
    pRowBuffer = pRowBuffer->pNextBuffer();
  }
  
  return bVerify;
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
