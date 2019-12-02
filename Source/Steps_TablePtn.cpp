/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "Steps.h"

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
  vAddStep("tablePtn.isLocked"_Stz,         &Step::vTablePtnIsLocked);
}

/*----------------------------------------------------------------------------*/
/* tablePtn.attemptGrabRead -- attempt to grabp read lock on table partition  */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAttemptGrabRead(void)
{
  ENTRY(); // ENTRY("vTablePtnAttemptGrabRead"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // attempt to grab the read lock
  bool bSuccess = pTablePtn->bGrabRead();
  
  m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.attemptGrabWrite -- attempt to grab write lock on table partition */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAttemptGrabWrite(void)
{
  ENTRY(); // ENTRY("vTablePtnAttemptGrabWrite"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // attempt to grab the write lock
  bool bSuccess = pTablePtn->bGrabWrite();
  
  m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.dropRead -- drop a read lock on the partition of a table          */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDropRead(void)
{
  ENTRY(); // ENTRY("vTablePtnDropRead"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
    
  // drop the read lock
  pTablePtn->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.dump -- dump partition id                                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDump(void)
{
  ENTRY(); // ENTRY("vTablePtnDump"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pTablePtn->iPartitionId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.dropWrite -- drop a write lock on the partition of a table        */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDropWrite(void)
{
  ENTRY(); // ENTRY("vTablePtnDropWrite"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // drop the write lock
  pTablePtn->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn.isLocked -- test if a table partition is locked                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnIsLocked(void)
{
  ENTRY(); // ENTRY("vTablePtnIsLocked"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  bool bIsLocked = pTablePtn->bIsLocked();
  
  m_pDataStack->vPushInt(bIsLocked);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
