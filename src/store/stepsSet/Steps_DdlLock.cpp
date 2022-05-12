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
  vAddStep("[ddlLock].assertNone"_Stz, &Step::vDdlLockAssertNone);
  vAddStep("[ddlLock].dropRead"_Stz,   &Step::vDdlLockDropRead);
  vAddStep("[ddlLock].dropWrite"_Stz,  &Step::vDdlLockDropWrite);
  vAddStep("[ddlLock].grabRead"_Stz,   &Step::vDdlLockGrabRead);
  vAddStep("[ddlLock].grabWrite"_Stz,  &Step::vDdlLockGrabWrite);
  vAddStep("[ddlLock].hasLock?"_Stz,   &Step::vDdlLockHasLock);
  vAddStep("[ddlLock].hasRead?"_Stz,   &Step::vDdlLockHasRead);
  vAddStep("[ddlLock].hasWrite?"_Stz,  &Step::vDdlLockHasWrite);
  vAddStep("[ddlLock].testDrop"_Stz,   &Step::vDdlLockTestDrop);
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].assertNone -- check for no ddl lock                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockAssertNone(void)
{
  ENTRY();

  if (LH_HAVE_NO_LOCK != m_pLogicUnit->m_eDdlLocked)
    THROW("[ddlLock].assertNone : register still has ddl locked"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].dropRead -- drop ddl lock for read                               */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropRead(void)
{
  ENTRY();

  AM::MASTERLOCK::vDropDdlReadLock(m_pLogicUnit);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].dropWrite -- drop ddl lock for write                             */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropWrite(void)
{
  ENTRY();
    
  AM::MASTERLOCK::vDropDdlWriteLock(m_pLogicUnit);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].grabRead -- grab ddl lock for read                               */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabRead(void)
{
  ENTRY();
    
  AM::MASTERLOCK::vGrabDdlReadLock(m_pLogicUnit);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].grabWrite -- grab ddl lock for write                             */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabWrite(void)
{
  ENTRY();
    
  AM::MASTERLOCK::vGrabDdlWriteLock(m_pLogicUnit);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].hasLock? -- does the logic unit have a ddl read or write lock?   */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockHasLock(void)
{
  ENTRY();
    
  if (LH_HAVE_NO_LOCK == m_pLogicUnit->m_eDdlLocked)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].hasRead? -- does the logic unit have a ddl read lock?            */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockHasRead(void)
{
  ENTRY();
    
  if (LH_HAVE_READ_LOCK == m_pLogicUnit->m_eDdlLocked)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].hasWrite? -- does the logic unit have a ddl write lock?          */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockHasWrite(void)
{
  ENTRY();
    
  if (LH_HAVE_WRITE_LOCK == m_pLogicUnit->m_eDdlLocked)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ddlLock].testDrop -- test for ddl lock and drop if locked by task         */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockTestDrop(void)
{
  ENTRY();
    
  if (m_pLogicUnit)
    AM::MASTERLOCK::vDdlLockTestDrop(m_pLogicUnit);
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
