/*============================================================================*/
/* Steps_Expressions.cpp - Expressions step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Result.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Expressions(void)
{
  vAddStep("STORE::lockSample"_Stz,   &Step::vExpressionsLockSample);
}

/*----------------------------------------------------------------------------*/
/* STORE::lockSample -- sample the lock stats                               */
/*----------------------------------------------------------------------------*/
void Step::vExpressionsLockSample(void)
{
  ENTRY();

  StreamDef* pStreamDef = *( m_pJob->pLogicUnit_job() )->m_xpStreamDef;
    
  //InfoSet* pInfoSet = LockMetrics::pSample(pStreamDef);
  
  //m_pDataStack->vPushPtr(pInfoSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
