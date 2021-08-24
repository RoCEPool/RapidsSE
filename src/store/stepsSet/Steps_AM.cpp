/*============================================================================*/
/* Steps_AM.cpp - AM step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AM(void)
{
  vAddStep("AM::SHOW::bBoot"_Stz,       &Step::vAM_Show_Boot);
  vAddStep("AM::SHOW::bConnection"_Stz, &Step::vAM_Show_Connection);
  vAddStep("AM::SHOW::bRecurring"_Stz,  &Step::vAM_Show_Recurring);
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bBoot -- <show_boot> ?                                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Boot(void)
{
  ENTRY();

  b8 bBoot = AM::SHOW::bBoot();
  
  m_pDataStack->vPushInt(bBoot);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bConnection -- <show_connection> ?                               */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Connection(void)
{
  ENTRY();

  b8 bConnection = AM::SHOW::bConnection();
  
  m_pDataStack->vPushInt(bConnection);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bRecurring -- <show_recurring> ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Recurring(void)
{
  ENTRY();

  b8 bRecurring = AM::SHOW::bRecurring();
  
  m_pDataStack->vPushInt(bRecurring);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
