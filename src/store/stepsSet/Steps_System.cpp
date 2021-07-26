/*============================================================================*/
/* Steps_System.cpp - system step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_System(void)
{
  vAddStep("system::started@"_Stz,          &Step::vSystemStartedFetch);
  vAddStep("system::started!"_Stz,          &Step::vSystemStartedStore);
}

/*----------------------------------------------------------------------------*/
/* system::started@ -- get grid started                                       */
/*----------------------------------------------------------------------------*/
void Step::vSystemStartedFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::SYSTEM::bStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* system::started! -- set grid started                                       */
/*----------------------------------------------------------------------------*/
void Step::vSystemStartedStore(void)
{
  ENTRY();
  
  i8 iStarted = m_pDataStack->iPopInt(__func__);
  
  AM::SYSTEM::vStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
