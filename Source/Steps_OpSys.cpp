/*============================================================================*/
/* Steps_OpSys.cpp - opsys step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_OpSys(void)
{
  vAddStep("opsys::isLinux"_Stz,  &Step::vOpSysIsLinux);
  vAddStep("opsys::isMacOS"_Stz,  &Step::vOpSysIsMacOs);
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vOpSysIsLinux(void)
{
  ENTRY();
  
#ifdef WITH_LINUX
  m_pDataStack->vPushInt(true);
#else
  m_pDataStack->vPushInt(false);
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vOpSysIsMacOs(void)
{
  ENTRY();

#ifdef WITH_APPLE
  m_pDataStack->vPushInt(true);
#else
  m_pDataStack->vPushInt(false);
#endif

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
