/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TablePtn* DBobjects::_pTablePtn(void* aAddr)
{
  TablePtn* pTablePtn = reinterpret_cast<TablePtn*>(aAddr);
  
  RASSERT( !pTablePtn || CN_TABLEPTN == pTablePtn->eClass() );
  
  return pTablePtn;
}

/*----------------------------------------------------------------------------*/
/* dump partition                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_TABLEPTN);
  RASSERT(pObject->eClass() == CN_TABLEPTN);
  
  TablePtn* pPartition = reinterpret_cast<TablePtn*>(pObject);
  
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
void* TablePtn::operator new(size_t iSize)
{
  RASSERT(iSize);
  void* pObject = Heap::pAllocate(CN_TABLEPTN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TablePtn::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  Object* pObject = reinterpret_cast<Object*>(pBufferAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TablePtn::TablePtn(void)
: Object(CN_TABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_xLock.vInitLock(LK_PARTITION);
  m_iPartitionId = -1;
  
  m_nBuffersLoadable = C_UNKNOWN;
  m_nBuffersLoaded   = 0;

  m_pIndexTreeVec  = new IndexTreeVec;
  
  _checkInitialised_(CN_TABLEPTN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_pIndexTreeVec->nItems() < C_INDEXES_PER_TABLE);
  m_pIndexTreeVec->vAdd(pIndexTree);
}

/*----------------------------------------------------------------------------*/
/* set number of loadable buffers                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vBuffersLoadable(i8 nBuffersLoadable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(C_UNKNOWN == m_nBuffersLoadable);
  
  m_nBuffersLoadable = nBuffersLoadable;
}

/*----------------------------------------------------------------------------*/
/* have all buffers been loaded?                                              */
/*----------------------------------------------------------------------------*/
bool TablePtn::bBuffersLoaded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  //ASSERT(C_UNKNOWN == m_nBuffersLoadable);
  
  vTrace("m_nBuffersLoadable %lld m_nBuffersLoaded %lld\n"_Stz,
         m_nBuffersLoadable,
         m_nBuffersLoaded);

  if (m_nBuffersLoaded == m_nBuffersLoadable)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* drop a read lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  m_xLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop a write lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a read lock on the partition                           */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  return m_xLock.bGrabRead();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a write lock on the partition                          */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_xLock.bGrabWrite();
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
/* increment buffers loaded so far                                            */
/*----------------------------------------------------------------------------*/
void TablePtn::vIncBuffersLoaded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_nBuffersLoaded++;
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
  
  return m_xLock.bIsLocked();
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
  
  EntryBuffer* pEntryBuffer = pHiddenIndexTree->pFirstL0Buffer();
  
  while (pEntryBuffer) {
    
    Entry* pEntry = pEntryBuffer->pFirstEntry();
    
//    while (pEntry) {
//
//      // insert entry
//      if ( ! pIndexTree->bInsertEntry(pEntry))
//        return false;
//
//      // get next entry
//      pEntry = pEntryBuffer->pNextEntry(pEntry, 0);
//    }
   
    pIndexTree->vInsertEntriesFromBuffer(pEntryBuffer, pEntry);
    
    pEntryBuffer = pEntryBuffer->pNextBuffer();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* reset the buffer load members                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vResetLoad(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_nBuffersLoadable = C_UNKNOWN;
  m_nBuffersLoaded = 0;
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
