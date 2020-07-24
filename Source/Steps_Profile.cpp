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
  vAddStep("Profile.name!"_Stz, &Step::vProfileNameStore);
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
/* Profile.name! -- store profile name                                        */
/*----------------------------------------------------------------------------*/
void Step::vProfileNameStore(void)
{
  ENTRY();
  
  s256 sName;
  m_pDataStack->vPopStr(sName, __func__);
  
  Profile* pProfile = m_pTask->pProfile();
  pProfile->vOpName(sName);
  
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
