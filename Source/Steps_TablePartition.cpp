/*============================================================================*/
/* Steps_TablePartition.cpp - table partition step group                      */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TablePartition(void)
{
  vAddPrimitive("partition.dropRead"_Stz,         &vPartitionDropRead,    false);
  vAddPrimitive("partition.dropWrite"_Stz,        &vPartitionDropWrite,   false);
  vAddPrimitive("partition.attemptGrabRead"_Stz,  &vPartitionAttemptGrabRead,false);
  vAddPrimitive("partition.attemptGrabWrite"_Stz,&vPartitionAttemptGrabWrite,false);
}

/*----------------------------------------------------------------------------*/
/* partition.dropRead -- drop a read lock on the partition of a table         */
/*----------------------------------------------------------------------------*/
void Task::vPartitionDropRead(MP_Task mpTask)
{
  ENTRY("vPartitionDropRead"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("partition.dropRead"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("partition.dropRead"_Stz);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  pTable->pPartition(iSlot)->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* partition.dropWrite -- drop a write lock on the partition of a table       */
/*----------------------------------------------------------------------------*/
void Task::vPartitionDropWrite(MP_Task mpTask)
{
  ENTRY("vPartitionDropWrite"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("partition.dropWrite"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("partition.dropWrite"_Stz);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  pTable->pPartition(iSlot)->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* partition.attemptGrabRead -- attempt to grabp read lock on table partition */
/*----------------------------------------------------------------------------*/
void Task::vPartitionAttemptGrabRead(MP_Task mpTask)
{
  ENTRY("vPartitionAttemptGrabRead"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("partition.attemptGrabRead"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("partition.attemptGrabRead"_Stz);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  bool bGrabRead = pTable->pPartition(iSlot)->bGrabRead();
  
  mpTask->m_pDataStack->vPushInt(bGrabRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* partition.attemptGrabWrite -- attempt to grab write lock on table partition*/
/*----------------------------------------------------------------------------*/
void Task::vPartitionAttemptGrabWrite(MP_Task mpTask)
{
  ENTRY("vPartitionAttemptGrabWrite"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("partition.attemptGrabWrite"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("partition.attemptGrabWrite"_Stz);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  bool bGrabWrite = pTable->pPartition(iSlot)->bGrabWrite();
  
  mpTask->m_pDataStack->vPushInt(bGrabWrite);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
