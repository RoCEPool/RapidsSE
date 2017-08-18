/*============================================================================*/
/* Partition.cpp - table partition                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Grid*        gpGrid;
extern AttachMap*   gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump partition                                                             */
/*----------------------------------------------------------------------------*/
void Partition::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_PARTITION);
  ASSERT(pObject->eClass() == CN_PARTITION);
  
  Partition* pPartition = reinterpret_cast<Partition*>(pObject);
  
  vTrace("m_iPartitionId %lld, DataVec %p\n"_Stz,
         pPartition->m_iPartitionId, pPartition->m_pIdxDataVec);
}

/*----------------------------------------------------------------------------*/
/* convert partition number to node number                                    */
/*----------------------------------------------------------------------------*/
i8 Partition::iPartitionToNode(i8 iPartition)
{
  i8 iNode = iPartition % gpGrid->iNodes();
  
  return iNode;
}

/*----------------------------------------------------------------------------*/
/* convert partition number to slot number                                    */
/*----------------------------------------------------------------------------*/
i8 Partition::iPartitionToSlot(i8 iPartition)
{
  i8 iSlot = iPartition / gpGrid->iNodes();
  
  return iSlot;
}

/*----------------------------------------------------------------------------*/
/* convert slot number to partition number                                    */
/*----------------------------------------------------------------------------*/
i8 Partition::iSlotToPartition(i8 iSlot)
{
  i8 iPartition = (iSlot * gpGrid->iNodes()) + gpGrid->iThisNode();
  
  return iPartition;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Partition::operator new(size_t iSize)
{
  RASSERT(iSize);
  void* pObject = Heap::pAllocate(CN_PARTITION);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Partition::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  Object* pObject = reinterpret_cast<Object*>(pBufferAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Partition::Partition(void)
: Object(CN_PARTITION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  m_iPartitionId = -1;
  m_pIdxDataVec  = new IdxDataVec;
  
  _checkInitialised_(CN_PARTITION);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partition::~Partition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
void Partition::vAddIndex(IndexData* pIndexData)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  ASSERT(m_pIdxDataVec->nItems() < C_INDEXES_PER_TABLE);
  m_pIdxDataVec->vAdd(pIndexData);
}

/*----------------------------------------------------------------------------*/
/* get this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
i8 Partition::iPartitionId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  return m_iPartitionId;
}

/*----------------------------------------------------------------------------*/
/* set this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
void Partition::vPartitionId(i8 iPartitionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  m_iPartitionId = iPartitionId;
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
void Partition::vPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  IndexData* pHiddenIndexData = this->pHiddenIndexData();
  IndexData* pIndexData = this->pIndexData(iIndexNo);
  
  EntryBuffer* pEntryBuffer = pHiddenIndexData->pFirstBuffer();
  
  while (pEntryBuffer) {
    
    Entry* pEntry = pEntryBuffer->pFirstEntry();
    
    while (pEntry) {
      
      // insert entry
      pIndexData->vInsertEntry(pEntry);
      
      // get next entry
      pEntry = pEntryBuffer->pNextEntry(pEntry);
    }
   
    pEntryBuffer = pEntryBuffer->pNextBuffer();
  }
}


/*----------------------------------------------------------------------------*/
/* get indexData                                                              */
/*----------------------------------------------------------------------------*/
IndexData* Partition::pIndexData(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  IndexData* pIndexData = nullptr;
  i8 nIndexes = m_pIdxDataVec->nItems();
  
  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    pIndexData = m_pIdxDataVec->pGet(i_Index);
    Index* pIndex = pIndexData->pIndex();
    if (pIndex->iIndexNo() == iIndexNo)
      break;
  }
  
  return pIndexData;
}

/*----------------------------------------------------------------------------*/
/* get hidden indexData                                                       */
/*----------------------------------------------------------------------------*/
IndexData* Partition::pHiddenIndexData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  IndexData* pIndexData = nullptr;
  pIndexData = m_pIdxDataVec->pGet(0);
  
  return pIndexData;
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IdxDataVec* Partition::pIdxDataVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  return m_pIdxDataVec;
}

/*----------------------------------------------------------------------------*/
/* tidy up                                                                    */
/*----------------------------------------------------------------------------*/
void Partition::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  // delete the index data vector
  delete m_pIdxDataVec;
}

/*----------------------------------------------------------------------------*/
/* tidy up                                                                    */
/*----------------------------------------------------------------------------*/
void Partition::vTidyUpIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  IndexData* pIndexData = this->pIndexData(iIndexNo);
  
  // remove the index from the IdxDataVec
  m_pIdxDataVec->vRemove(pIndexData);
  delete pIndexData;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Partition::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
