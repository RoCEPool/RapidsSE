/*============================================================================*/
/* Steps_Metrics.cpp - Metrics step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Expressions(void)
{
  vAddStep("STORE::lockSample"_Stz,   &Step::vExpressionsLockSample);
}

/*----------------------------------------------------------------------------*/
/* STORE::lockSample -- sample the lock stats                                 */
/*----------------------------------------------------------------------------*/
void Step::vExpressionsLockSample(void)
{
  ENTRY();

  //StreamDef* pStreamDef = *m_pLogicUnit->m_xpStreamDefLft;
    
  //InfoSet* pInfoSet = LockMetrics::pSample(pStreamDef);
  
  //m_pDataStack->vPushPtr(pInfoSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
