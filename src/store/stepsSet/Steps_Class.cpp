/*============================================================================*/
/* Steps_Class.cpp - class steps group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Class(void)
{
  vAddStep("class::name"_Stz,      &Step::vClassName);
}

/*----------------------------------------------------------------------------*/
/* class::name -- translate class no to name                                  */
/*----------------------------------------------------------------------------*/
void Step::vClassName(void)
{
  ENTRY();
  
  i8 iClassNo = m_pDataStack->iPopInt(__func__);
  
  s32 sClassName;
  
  sClassName = aCN[iClassNo];
  
  m_pDataStack->vPushStr(sClassName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
