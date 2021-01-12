/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "ReadApi.hpp"
#include "TablePtn.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePartition2(void)
{
  vAddStep("tablePtn[]"_Stz,                  &Step::vTablePtnGet);
  
  vAddStep("tablePtn[].appendBuffer"_Stz,     &Step::vTablePtnAppendBuffer);
  vAddStep("tablePtn[].populateIndex"_Stz,    &Step::vTablePtnPopulateIndex);
  vAddStep("tablePtn[].purge"_Stz,            &Step::vTablePtnPurge);
  vAddStep("tablePtn[].root"_Stz ,            &Step::vTablePtnRoot);
  vAddStep("tablePtn[].set"_Stz  ,            &Step::vTablePtnSet);
  vAddStep("tablePtn[].truncate"_Stz,         &Step::vTablePtnTruncate);
  vAddStep("tablePtn[].truncateIndex"_Stz,    &Step::vTablePtnTruncateIndex);
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].appendBuffer -- append a buffer into the tablePtn               */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAppendBuffer(void)
{
  ENTRY();
  
  // get the CrwBuffer
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
