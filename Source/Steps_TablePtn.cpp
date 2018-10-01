/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_TablePartition(void)
{
  vAddStep("tablePtn.dump"_Stz,             &Task::vTablePtnDump,            false);
  vAddStep("tablePtn.attemptGrabRead"_Stz , &Task::vTablePtnAttemptGrabRead, false);
  vAddStep("tablePtn.attemptGrabWrite"_Stz, &Task::vTablePtnAttemptGrabWrite,false);
  vAddStep("tablePtn.dropRead"_Stz,         &Task::vTablePtnDropRead,        false);
  vAddStep("tablePtn.dropWrite"_Stz,        &Task::vTablePtnDropWrite,       false);
  vAddStep("tablePtn.isLocked"_Stz,         &Task::vTablePtnIsLocked,        false);
}

/*----------------------------------------------------------------------------*/
/* tablePtn.dump -- dump partition id                                         */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnDump(MP_Task mpTask)
{
  ENTRY("vTablePtnDump"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.dump"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  mpTask->m_pDataStack->vPushInt(pTablePtn->iPartitionId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.attemptGrabRead -- attempt to grabp read lock on table partition  */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnAttemptGrabRead(MP_Task mpTask)
{
  ENTRY("vTablePtnAttemptGrabRead"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.attemptGrabRead"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  // attempt to grab the read lock
  bool bSuccess = pTablePtn->bGrabRead();
  
  mpTask->m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.attemptGrabWrite -- attempt to grab write lock on table partition */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnAttemptGrabWrite(MP_Task mpTask)
{
  ENTRY("vTablePtnAttemptGrabWrite"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.attemptGrabWrite"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  // attempt to grab the write lock
  bool bSuccess = pTablePtn->bGrabWrite();
  
  mpTask->m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.dropRead -- drop a read lock on the partition of a table          */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnDropRead(MP_Task mpTask)
{
  ENTRY("vTablePtnDropRead"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.dropRead"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
    
  // drop the read lock
  pTablePtn->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.dropWrite -- drop a write lock on the partition of a table        */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnDropWrite(MP_Task mpTask)
{
  ENTRY("vTablePtnDropWrite"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("-"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  // drop the write lock
  pTablePtn->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.isLocked -- test if a table partition is locked                   */
/*----------------------------------------------------------------------------*/
void Task::vTablePtnIsLocked(MP_Task mpTask)
{
  ENTRY("vTablePtnIsLocked"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.isLocked"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  bool bIsLocked = pTablePtn->bIsLocked();
  
  mpTask->m_pDataStack->vPushInt(bIsLocked);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
