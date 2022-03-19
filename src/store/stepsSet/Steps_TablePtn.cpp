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
#include "TablePtn.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePtn(void)
{
  vAddStep("TablePtn[]"_Stz,                  &Step::vTablePtnGet);
  
  vAddStep("TablePtn[].appendBuffer"_Stz,     &Step::vTablePtnAppendBuffer);
  vAddStep("TablePtn[].attemptGrabRead"_Stz , &Step::vTablePtnAttemptGrabRead);
  vAddStep("TablePtn[].attemptGrabWrite"_Stz, &Step::vTablePtnAttemptGrabWrite);
  vAddStep("TablePtn[].dropRead"_Stz,         &Step::vTablePtnDropRead);
  vAddStep("TablePtn[].dropWrite"_Stz,        &Step::vTablePtnDropWrite);
  vAddStep("TablePtn[].populateIndex"_Stz,    &Step::vTablePtnPopulateIndex);
  vAddStep("TablePtn[].root"_Stz ,            &Step::vTablePtnRoot);
  vAddStep("TablePtn[].set"_Stz  ,            &Step::vTablePtnSet);
  vAddStep("TablePtn[].truncate"_Stz,         &Step::vTablePtnTruncate);
  vAddStep("TablePtn[].truncateIndex"_Stz,    &Step::vTablePtnTruncateIndex);
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].appendBuffer -- append a buffer into the tablePtn               */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAppendBuffer(void)
{
  ENTRY();
  
  DataBuffer* pDataBuffer = _pDataBuffer(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pDataBuffer->pNextItem());
  ASSERT(! pDataBuffer->pPrevItem());

  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);
  
  // append the buffer to the tablePtn
  
  eCN eClassNo = pDataBuffer->eGetClassNo();
  pTablePtn->vAppendBuffer(pDataBuffer);

//  if (CN_CRWBUFFER == eClassNo) {
//    CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);
//    pTablePtn->vAppendBuffer(pCrwBuffer);
//  }
//  else if (CN_ROWBUFFER == eClassNo) {
//    RowBuffer* pRowBuffer = _pRowBuffer(pDataBuffer);
//    NEVER_REACH;
//    //pTablePtn->vAppendBuffer(pRowBuffer);
//  }
//  else {
//    NEVER_REACH;
//  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].attemptGrabRead -- attempt to grab read lock on table partition */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAttemptGrabRead(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);

  // attempt to grab the read lock
  bool bSuccess = pTablePtn->bGrabRead();
  
  m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].attemptGrabWrite -- attempt grab write lock on table partition  */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAttemptGrabWrite(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);

  // attempt to grab the write lock
  bool bSuccess = pTablePtn->bGrabWrite();
  
  m_pDataStack->vPushInt(bSuccess);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].dropRead -- drop a read lock on the partition of a table        */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDropRead(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);

    
  // drop the read lock
  pTablePtn->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].dropWrite -- drop write lock on the partition of a tabl  e      */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnDropWrite(void)
{
  ENTRY();
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);

  // drop the write lock
  pTablePtn->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[] -- get table                                                    */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].populateIndex -- populate index of this table ptn               */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnPopulateIndex(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eGetClassNo());
  
  i8 iIndexNo = pIndex->iIndexNo();
  
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  
  bool bPopulated = pTablePtn->bPopulateIndex(iIndexNo);
  
  m_pDataStack->vPushInt(bPopulated ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].root -- get the tablePtn root                                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnRoot(void)
{
  ENTRY();
  // Nobody should call this
  vTrace(">>>>>>>>>>>>>>>>>>>>>  Step::vTablePtnRoot\n"_Stz);
  NEVER_REACH;
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);
  
  //m_pDataStack->vPushPtr(pTablePtn->pRootCrwBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].set -- set the tablePtn                                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].truncate -- truncate the tablePtn for the table                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnTruncate(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  pTablePtn->vTruncate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TablePtn[].truncateIndex -- truncate the tablePtn for the index            */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnTruncateIndex(void)
{
  ENTRY();
  
  // get the index to be truncated
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  pTablePtn->vTruncateIndex(pIndex);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
