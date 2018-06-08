/*============================================================================*/
/* Steps_TablePtn.cpp - table partition step group                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TablePartition(void)
{
  vAddPrimitive("tablePtn::dropRead"_Stz,        &vTablePtnDropRead,    false);
  vAddPrimitive("tablePtn::dropWrite"_Stz,       &vTablePtnDropWrite,   false);
  vAddPrimitive("tablePtn::attemptGrabRead"_Stz, &vTablePtnAttemptGrabRead,false);
  vAddPrimitive("tablePtn::attemptGrabWrite"_Stz,&vTablePtnAttemptGrabWrite,false);
}

/*----------------------------------------------------------------------------*/
/* tablePtn::dropRead -- drop a read lock on the partition of a table        */
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
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  pTable->pPartition(iSlot)->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn::dropWrite -- drop a write lock on the partition of a table      */
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
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  pTable->pPartition(iSlot)->vDropWrite();
  
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
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  bool bGrabRead = pTable->pPartition(iSlot)->bGrabRead();
  
  mpTask->m_pDataStack->vPushInt(bGrabRead);
  
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
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  bool bGrabWrite = pTable->pPartition(iSlot)->bGrabWrite();
  
  mpTask->m_pDataStack->vPushInt(bGrabWrite);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
