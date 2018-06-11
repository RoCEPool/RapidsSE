/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TablePartition(void)
{
  vAddPrimitive("tablePtn::dump"_Stz,            &vTablePtnDump,            false);
  vAddPrimitive("tablePtn::attemptGrabRead"_Stz, &vTablePtnAttemptGrabRead, false);
  vAddPrimitive("tablePtn::attemptGrabWrite"_Stz,&vTablePtnAttemptGrabWrite,false);
  vAddPrimitive("tablePtn::dropRead"_Stz,        &vTablePtnDropRead,        false);
  vAddPrimitive("tablePtn::dropWrite"_Stz,       &vTablePtnDropWrite,       false);
  vAddPrimitive("tablePtn::isLocked"_Stz,        &vTablePtnIsLocked,        false);
}

/*----------------------------------------------------------------------------*/
/* tablePtn::dump -- dump partition id                                        */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnDump(MP_Task mpTask)
{
  ENTRY("vTablePtnDump"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("tablePtn::dump"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("tablePtn::dump"_Stz);
  
  // get the slot no
  i8 i_Slot = TablePtn::iPartitionToSlot(iPartition);
  
  // get the table partition
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);
  
  mpTask->m_pDataStack->vPushInt(pTablePtn->iPartitionId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn::attemptGrabRead -- attempt to grabp read lock on table partition */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnAttemptGrabRead(MP_Task mpTask)
{
  ENTRY("vTablePtnAttemptGrabRead"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("tablePtn::attemptGrabRead"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("tablePtn::attemptGrabRead"_Stz);
  
  // get the slot no
  i8 i_Slot = TablePtn::iPartitionToSlot(iPartition);
  
  // get the table partition
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);
  
  // attempt to grab the read lock
  bool bSuccess = pTablePtn->bGrabRead();
  
  mpTask->m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn::attemptGrabWrite -- attempt to grab write lock on table partition*/
/*----------------------------------------------------------------------------*/
void Task::vTablePtnAttemptGrabWrite(MP_Task mpTask)
{
  ENTRY("vTablePtnAttemptGrabWrite"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("tablePtn::attemptGrabWrite"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("tablePtn::attemptGrabWrite"_Stz);
  
  // get the slot no
  i8 i_Slot = TablePtn::iPartitionToSlot(iPartition);
  
  // get the table partition
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);
  
  // attempt to grab the write lock
  bool bSuccess = pTablePtn->bGrabWrite();
  
  mpTask->m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn::dropRead -- drop a read lock on the partition of a table         */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnDropRead(MP_Task mpTask)
{
  ENTRY("vTablePtnDropRead"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("tablePtn::dropRead"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("tablePtn::dropRead"_Stz);
  
  // get the slot no
  i8 i_Slot = TablePtn::iPartitionToSlot(iPartition);
  
  // get the table partition
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);
  
  // drop the read lock
  pTablePtn->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn::dropWrite -- drop a write lock on the partition of a table       */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnDropWrite(MP_Task mpTask)
{
  ENTRY("vTablePtnDropWrite"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("tablePtn::dropWrite"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("tablePtn::dropWrite"_Stz);
  
  // get the slot no
  i8 i_Slot = TablePtn::iPartitionToSlot(iPartition);
  
  // get the table partition
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);
  
  // drop the write lock
  pTablePtn->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn::isLocked -- test if a table partition is locked                  */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnIsLocked(MP_Task mpTask)
{
  ENTRY("vTablePtnIsLocked"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("tablePtn::dropWrite"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("tablePtn::dropWrite"_Stz);
  
  // get the slot no
  i8 i_Slot = TablePtn::iPartitionToSlot(iPartition);
  
  // get the table partition
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);

  bool bIsLocked = pTablePtn->bIsLocked();
  
  mpTask->m_pDataStack->vPushInt(bIsLocked);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
