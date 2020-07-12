/*============================================================================*/
/* Steps_DdlLock.cpp - ddl lock step group                                    */
/*============================================================================*/

#include "Steps.h"

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
  
  ASSERT(LH_HAVE_READ_LOCK == m_pLogicUnit->m_eDdlLocked);


  AM::pDdlLock()->vDropRead();
  m_pLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].dropWrite -- drop ddl lock for write                             */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropWrite(void)
{
  ENTRY();
  
  ASSERT(LH_HAVE_WRITE_LOCK == m_pLogicUnit->m_eDdlLocked);
  
  AM::pDdlLock()->vDropWrite();
  m_pLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].grabRead -- grab ddl lock for read                               */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabRead(void)
{
  ENTRY();
  
  ASSERT(! m_pLogicUnit->m_eDdlLocked);
  
  AM::pDdlLock()->vGrabRead();
  m_pLogicUnit->m_eDdlLocked = LH_HAVE_READ_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].grabWrite -- grab ddl lock for write                             */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabWrite(void)
{
  ENTRY();
  
  ASSERT(! m_pLogicUnit->m_eDdlLocked);
  
  AM::pDdlLock()->vGrabWrite();
  m_pLogicUnit->m_eDdlLocked = LH_HAVE_WRITE_LOCK;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].none -- check for no ddl lock                                    */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockNone(void)
{
  ENTRY();

  if (LH_HAVE_NO_LOCK != m_pLogicUnit->m_eDdlLocked)
    THROW("DdlLock[].none : register still has ddl locked"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].testDrop -- test for ddl lock and drop if locked by task         */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockTestDrop(void)
{
  ENTRY();
  
  if (LH_HAVE_READ_LOCK == m_pLogicUnit->m_eDdlLocked) {
    
    AM::pDdlLock()->vDropRead();
    m_pLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;
  }
  else
  if (LH_HAVE_WRITE_LOCK == m_pLogicUnit->m_eDdlLocked) {
    
    AM::pDdlLock()->vDropWrite();
    m_pLogicUnit->m_eDdlLocked = LH_HAVE_NO_LOCK;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
