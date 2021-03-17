/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Index.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "ReadApi.hpp"
#include "TablePtn.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePtn(void)
{
  vAddStep("tablePtn[]"_Stz,                  &Step::vTablePtnGet);
  
  vAddStep("tablePtn[].appendBuffer"_Stz,     &Step::vTablePtnAppendBuffer);
  vAddStep("tablePtn[].populateIndex"_Stz,    &Step::vTablePtnPopulateIndex);
  vAddStep("tablePtn[].purge"_Stz,            &Step::vTablePtnPurge);
  vAddStep("tablePtn[].root"_Stz ,            &Step::vTablePtnRoot);
  vAddStep("tablePtn[].set"_Stz  ,            &Step::vTablePtnSet);
  vAddStep("tablePtn[].truncate"_Stz,         &Step::vTablePtnTruncate);
  vAddStep("tablePtn[].truncateIndex"_Stz,    &Step::vTablePtnTruncateIndex);
  
  
  vAddStep("tablePtn[].attemptGrabRead"_Stz , &Step::vTablePtnAttemptGrabRead);
  vAddStep("tablePtn[].attemptGrabWrite"_Stz, &Step::vTablePtnAttemptGrabWrite);
  vAddStep("tablePtn[].dropRead"_Stz,         &Step::vTablePtnDropRead);
  vAddStep("tablePtn[].dropWrite"_Stz,        &Step::vTablePtnDropWrite);
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].appendBuffer -- append a buffer into the tablePtn               */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAppendBuffer(void)
{
  ENTRY();
  
  // get the CrwBuffer
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pCrwBuffer->pNextItem());
  ASSERT(! pCrwBuffer->pPrevItem());

  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*( m_pJob->pLogicUnit_job() )->m_xpTablePtn);
  
  // append the buffer to the tablePtn
  pTablePtn->vAppendBuffer(pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[] -- get table                                                    */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].populateIndex -- populate index of this table ptn               */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnPopulateIndex(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIndexNo = pIndex->iIndexNo();
  
  TablePtn* pTablePtn = *( m_pJob->pLogicUnit_job() )->m_xpTablePtn;
  
  bool bPopulated = pTablePtn->bPopulateIndex(iIndexNo);
  
  m_pDataStack->vPushInt(bPopulated ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].purge -- purge prior versions of this table ptn                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnPurge(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = *( m_pJob->pLogicUnit_job() )->m_xpTablePtn;
  
  // get the oldest perspective
  t8 tOldestPsp = AM::pTxSet()->tOldestPsp();
  pTablePtn->pReadApi()->vPurgePriorVersions(tOldestPsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].root -- get the tablePtn root                                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnRoot(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = _pTablePtn(*( m_pJob->pLogicUnit_job() )->m_xpTablePtn);
  
  m_pDataStack->vPushPtr(pTablePtn->pRootCrwBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].set -- set the tablePtn                                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnSet(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_xpTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].truncate -- truncate the tablePtn for the table                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnTruncate(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = *( m_pJob->pLogicUnit_job() )->m_xpTablePtn;
  pTablePtn->vTruncate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].truncateIndex -- truncate the tablePtn for the index            */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnTruncateIndex(void)
{
  ENTRY();
  
  // get the index to be truncated
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  TablePtn* pTablePtn = *( m_pJob->pLogicUnit_job() )->m_xpTablePtn;
  pTablePtn->vTruncateIndex(pIndex);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* tablePtn[].attemptGrabRead -- attempt to grabp read lock on table partition  */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAttemptGrabRead(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*( m_pJob->pLogicUnit_job() )->m_xpTablePtn);

  // attempt to grab the read lock
  bool bSuccess = pTablePtn->bGrabRead();
  
  m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].attemptGrabWrite -- attempt to grab write lock on table partition */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAttemptGrabWrite(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*( m_pJob->pLogicUnit_job() )->m_xpTablePtn);

  // attempt to grab the write lock
  bool bSuccess = pTablePtn->bGrabWrite();
  
  m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].dropRead -- drop a read lock on the partition of a table          */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDropRead(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*( m_pJob->pLogicUnit_job() )->m_xpTablePtn);

    
  // drop the read lock
  pTablePtn->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].dropWrite -- drop a write lock on the partition of a table        */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDropWrite(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*( m_pJob->pLogicUnit_job() )->m_xpTablePtn);

  // drop the write lock
  pTablePtn->vDropWrite();
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
