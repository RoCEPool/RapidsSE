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
  m_pIdxVec      = new IdxVec;
  
  _checkInitialised_(CN_PARTITION);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partition::~Partition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
void Partition::vAddIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  ASSERT(m_pIdxVec->nItems() < C_INDEXES_PER_TABLE);
  m_pIdxVec->vAdd(pIndex);
}

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool Partition::bInsertRow(RowBuffer* pRowBuffer, TxRef* pTxRef, Str* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  Row* pRow;
  
  // insert the row in the buffer
  bool bResult = pRowBuffer->bInsertRow( pTxRef, pRecord, iLength, &pRow);
  
  // buffer is full, return
  if (! bResult )
    return false;
  
  // now insert an entry for the row in the indices of this table
  i8 nIndexes = m_pIdxVec->nItems();
  
  for (i8 iIndex=0; iIndex < nIndexes; iIndex++) {
    Index* pIndex = m_pIdxVec->pGet(iIndex);
    pIndex->vInsertEntry(pRow, pRowBuffer->nRows(), pRowBuffer->nBufferNo());
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
/* get data                                                                   */
/*----------------------------------------------------------------------------*/
Data* Partition::pData(void)
{
  return m_pData;
}

/*----------------------------------------------------------------------------*/
/* get index                                                                  */
/*----------------------------------------------------------------------------*/
Index* Partition::pIndex(i8 iIndex)
{
  return m_pIdxVec->pGet(iIndex);
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
