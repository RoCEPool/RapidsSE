/*============================================================================*/
/* Steps_Executable.cpp - executable step group                               */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Executable(void)
{
  vAddStep("executable::isDebug"_Stz,   &Step::vExecutableIsDebug);
  vAddStep("executable::isRelease"_Stz, &Step::vExecutableIsRelease);
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vExecutableIsDebug(void)
{
  ENTRY();
  
#ifdef WITH_DEBUG_VERSION
  m_pTask->m_pDataStack->vPushInt(true);
#else
  m_pTask->m_pDataStack->vPushInt(false);
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vExecutableIsRelease(void)
{
  ENTRY();

#ifdef WITH_RELEASE_VERSION
  m_pTask->m_pDataStack->vPushInt(true);
#else
  m_pTask->m_pDataStack->vPushInt(false);
#endif

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
