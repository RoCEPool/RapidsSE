/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePartition(void)
{
  
  vAddStep("tablePtn.attemptGrabRead"_Stz , &Step::vTablePtnAttemptGrabRead);
  vAddStep("tablePtn.attemptGrabWrite"_Stz, &Step::vTablePtnAttemptGrabWrite);
  vAddStep("tablePtn.dropRead"_Stz,         &Step::vTablePtnDropRead);
  vAddStep("tablePtn.dropWrite"_Stz,        &Step::vTablePtnDropWrite);
  vAddStep("tablePtn.dump"_Stz,             &Step::vTablePtnDump);
  vAddStep("tablePtn.Ingesting"_Stz ,       &Step::vTablePtnIngesting);
  vAddStep("tablePtn.Inserting"_Stz ,       &Step::vTablePtnInserting);
  vAddStep("tablePtn.isLocked"_Stz,         &Step::vTablePtnIsLocked);
  vAddStep("tablePtn.Loading"_Stz ,         &Step::vTablePtnLoading);
}

/*----------------------------------------------------------------------------*/
/* tablePtn.attemptGrabRead -- attempt to grabp read lock on table partition  */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAttemptGrabRead(MP_Task mpTask)
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
void Step::vTablePtnAttemptGrabWrite(MP_Task mpTask)
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
void Step::vTablePtnDropRead(MP_Task mpTask)
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
/* tablePtn.dump -- dump partition id                                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDump(MP_Task mpTask)
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
/* tablePtn.dropWrite -- drop a write lock on the partition of a table        */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDropWrite(MP_Task mpTask)
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
/* tablePtn.ingesting -- data ingested into tablePtn                          */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnIngesting(MP_Task mpTask)
{
  ENTRY("vTablePtnIngesting"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.ingesting"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  pTablePtn->vIngesting();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.insterting -- data inserted into tablePtn                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnInserting(MP_Task mpTask)
{
  ENTRY("vTablePtnInserting"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.insterting"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  pTablePtn->vInserting();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.isLocked -- test if a table partition is locked                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnIsLocked(MP_Task mpTask)
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

/*----------------------------------------------------------------------------*/
/* tablePtn.loading -- data loading into tablePtn                             */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnLoading(MP_Task mpTask)
{
  ENTRY("vTablePtnLoading"_Stz);
  
  Object* pPtrTablePtn;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTablePtn) )
    THROW_INVALID_DSTACK("tablePtn.loading"_Stz);
  TablePtn* pTablePtn = _pTablePtn(pPtrTablePtn);
  
  pTablePtn->vLoading();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
