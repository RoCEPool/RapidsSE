/*============================================================================*/
/* Steps_Debug.cpp - debug step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Debug(void)
{
  vAddStep("debug::breakPoint"_Stz,  &Step::vDebugTrap);
  vAddStep("debug::flag@"_Stz,       &Step::vDebugFlagFetch);
  vAddStep("debug::flag!"_Stz,       &Step::vDebugFlagStore);
  vAddStep("debug::trap"_Stz,        &Step::vDebugTrap);
}

/*----------------------------------------------------------------------------*/
/* debug::flag@ -- fetch debug flag                                           */
/*----------------------------------------------------------------------------*/
void Step::vDebugFlagFetch(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pRegSet->m_iDebugFlag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::flag! -- store debug flag                                           */
/*----------------------------------------------------------------------------*/
void Step::vDebugFlagStore(void)
{
  ENTRY();

  m_pRegSet->m_iDebugFlag = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::trap -- call debug trap                                              */
/*----------------------------------------------------------------------------*/
void Step::vDebugTrap(void)
{
  ENTRY();

  Correctness::vDebugPoint();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
