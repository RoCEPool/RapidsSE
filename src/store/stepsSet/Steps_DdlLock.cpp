/*============================================================================*/
/* Steps_DdlLock.cpp - ddl lock step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DdlLock(void)
{
  vAddStep("DdlLock[].dropRead"_Stz,  &Step::vDdlLockDropRead);
  vAddStep("DdlLock[].dropWrite"_Stz, &Step::vDdlLockDropWrite);
  vAddStep("DdlLock[].grabRead"_Stz,  &Step::vDdlLockGrabRead);
  vAddStep("DdlLock[].grabWrite"_Stz, &Step::vDdlLockGrabWrite);
  vAddStep("DdlLock[].none"_Stz,      &Step::vDdlLockNone);
  vAddStep("DdlLock[].testDrop"_Stz,  &Step::vDdlLockTestDrop);
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].dropRead -- drop ddl lock for read                               */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropRead(void)
{
  ENTRY();
  
  ASSERT(LH_HAVE_READ_LOCK == m_p_TaskLogicUnit->m_eDdlLocked);


  AM::MASTERLOCK::vDropDdlReadLock();
  m_p_TaskLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].dropWrite -- drop ddl lock for write                             */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropWrite(void)
{
  ENTRY();
  
  ASSERT(LH_HAVE_WRITE_LOCK == m_p_TaskLogicUnit->m_eDdlLocked);
  
  AM::MASTERLOCK::vDropDdlWriteLock();
  m_p_TaskLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].grabRead -- grab ddl lock for read                               */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabRead(void)
{
  ENTRY();
  
  ASSERT(! m_p_TaskLogicUnit->m_eDdlLocked);
  
  AM::MASTERLOCK::vGrabDdlReadLock();
  m_p_TaskLogicUnit->m_eDdlLocked = LH_HAVE_READ_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].grabWrite -- grab ddl lock for write                             */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabWrite(void)
{
  ENTRY();
  
  ASSERT(! m_p_TaskLogicUnit->m_eDdlLocked);
  
  AM::MASTERLOCK::vGrabDdlWriteLock();
  m_p_TaskLogicUnit->m_eDdlLocked = LH_HAVE_WRITE_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].none -- check for no ddl lock                                    */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockNone(void)
{
  ENTRY();

  if (! pJob() || ! pJob()->pJobLogicUnit()) {
    EXIT();
    return;
  }

  if (LH_HAVE_NO_LOCK != m_p_TaskLogicUnit->m_eDdlLocked)
    THROW("DdlLock[].none : register still has ddl locked"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].testDrop -- test for ddl lock and drop if locked by task         */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockTestDrop(void)
{
  ENTRY();
  
  if (! pJob() || ! pJob()->pJobLogicUnit()) {
    EXIT();
    return;
  }
  
  if (LH_HAVE_READ_LOCK == m_p_TaskLogicUnit->m_eDdlLocked) {
    
    AM::MASTERLOCK::vDropDdlReadLock();
    m_p_TaskLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;
  }
  else
  if (LH_HAVE_WRITE_LOCK == m_p_TaskLogicUnit->m_eDdlLocked) {
    
    AM::MASTERLOCK::vDropDdlWriteLock();
    m_p_TaskLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
