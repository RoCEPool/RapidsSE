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
void Partition::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_PARTITION);
  ASSERT(pObject->eClass() == CN_PARTITION);
  
  Partition* pPartition = reinterpret_cast<Partition*>(pObject);
  
  vTrace("Data %p\n"_Str,
         pPartition->m_pData);
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
  vFatal("fatal: Partition class cannot be new'd"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Partition::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: Partition class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Partition::Partition(void)
: Object(CN_PARTITION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  m_iPartitionId = -1;
  m_pData        = new Data;
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
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool Partition::bInsertRow(EntryBuffer* pRowBuffer, EntWrk& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  Entry* pRow;
  
  // insert the row in the buffer
  bool bResult = pRowBuffer->bInsertRow(xRowWrk, &pRow);
  
  // buffer is full, return
  if (! bResult )
    return false;
  
  // now insert an entry for the row in the indexes of this table
  i8 nIndexes = m_pIdxDataVec->nItems();
  
  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    IndexData* pIndexData = m_pIdxDataVec->pGet(i_Index);
    pIndexData->vInsertEntry(pRow, pRowBuffer->nEntries(), pRowBuffer->nBufferNo());
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
i8 Partition::iPartitionId(void)
{
  return m_iPartitionId;
}

/*----------------------------------------------------------------------------*/
/* set this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
void Partition::vPartitionId(i8 iPartitionId)
{
  m_iPartitionId = iPartitionId;
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
#ifdef HIDDEN_INDEX
void Partition::vPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  IndexData* pHiddenIndexData = this->pHiddenIndexData();
  IndexData* pIndexData = this->pIndexData(iIndexNo);
  
  EntryBuffer* pEntryBuffer = pHiddenIndexData->pFirstBuffer();
  i8 nBuffers = 0;
  
  while (pEntryBuffer) {
    
    bn nBufferNo = pEntryBuffer->nBufferNo();
    
    Entry* pEntry = pEntryBuffer->pFirstEntry();
    
    i8 nRowsInBuffer = 0;
    i8 iRowNo = 0;
    
    while (pEntry) {
      
      iRowNo++;
      pIndexData->vInsertEntry(pEntry, iRowNo, nBufferNo);
      
      pEntry = pEntryBuffer->pNextEntry(pEntry);
    }
   
    pEntryBuffer = pEntryBuffer->pNextBuffer();
  }
}

#else
void Partition::vPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  IndexData* pIndexData = this->pIndexData(iIndexNo);
  
  i8 nStrands = 0;
  i8 nBuffers = 0;
  
  for (EbStrand* pStrand = m_pData->pHeadStrand();
       pStrand;
       pStrand = pStrand->pNextStrand()) {
    
    nStrands++;
    nBuffers += pStrand->nEntryBuffers();
    
    i8 nRowsInBuffer = 0;
    i8 nRowBuffer = 0;
    
    for (EntryBuffer* pRowBuffer = pStrand->pFirstBuffer();
         pRowBuffer;
         pRowBuffer = (EntryBuffer*)pRowBuffer->pNextBuffer()) {
      
      pRowBuffer->vShow();
      
      nRowBuffer++;
      nRowsInBuffer = pRowBuffer->nEntries();
      
      bn nBufferNo = pRowBuffer->nBufferNo();
      
      i8 iRowNo = -1;
      for (Entry* pRow = pRowBuffer->pFirstRow();
           pRow;
           pRow = pRowBuffer->pNextRow(pRow)) {
        
        iRowNo++;
        pIndexData->vInsertEntry(pRow, iRowNo, nBufferNo);
        
        //if (nRowBuffer == 2) {
        //  vTrace("************************\n"_Str);
        //  pIndexData->vDump();
        //  vTrace("************************\n"_Str);
        //}

      } // row
      
      ASSERT(iRowNo + 1 == nRowsInBuffer);
      
    } // rowBuffer
    
    ASSERT(nRowBuffer = pStrand->nEntryBuffers());
    
  } // strand
  
  ASSERT(nStrands == m_pData->nEbStrands());
  
  //pIndexData->vDump();
  //ASSERT(pIndexData->bVerify());
}
#endif

/*----------------------------------------------------------------------------*/
/* get data                                                                   */
/*----------------------------------------------------------------------------*/
Data* Partition::pData(void)
{
  return m_pData;
}

/*----------------------------------------------------------------------------*/
/* get indexData                                                              */
/*----------------------------------------------------------------------------*/
IndexData* Partition::pIndexData(i8 iIndexNo)
{
  IndexData* pIndexData = nullptr;
  i8 nIndexes = m_pIdxDataVec->nItems();
  
  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    pIndexData = m_pIdxDataVec->pGet(i_Index);
    if (pIndexData->pIndex()->iIndexNo() == iIndexNo)
      break;
  }
  
  return pIndexData;
}

/*----------------------------------------------------------------------------*/
/* get hidden indexData                                                       */
/*----------------------------------------------------------------------------*/
IndexData* Partition::pHiddenIndexData(void)
{
  IndexData* pIndexData = nullptr;
  pIndexData = m_pIdxDataVec->pGet(0);
  
  return pIndexData;
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IdxDataVec* Partition::pIdxDataVec(void)
{
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
  
  // delete the data
  delete m_pData;
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
Footer* Partition::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
