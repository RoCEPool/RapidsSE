/*============================================================================*/
/* Steps_Profile.cpp - Profile step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Profile(void)
{
  vAddStep("Profile.end"_Stz,   &Step::vProfileEnd);
  vAddStep("Profile.start"_Stz, &Step::vProfileStart);
}

/*----------------------------------------------------------------------------*/
/* Profile.end -- end profile                                                 */
/*----------------------------------------------------------------------------*/
void Step::vProfileEnd(void)
{
  ENTRY();
  
  Profile* pProfile = m_pTask->pProfile();
  RU_Cpu* pRU_Cpu = pProfile->pRU_Cpu();
  pRU_Cpu->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Profile.start -- start profile                                             */
/*----------------------------------------------------------------------------*/
void Step::vProfileStart(void)
{
  ENTRY();
  
  Profile* pProfile = m_pTask->pProfile();
  RU_Cpu* pRU_Cpu = pProfile->pRU_Cpu();
  pRU_Cpu->vStart();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
