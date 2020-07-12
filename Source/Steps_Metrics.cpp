/*============================================================================*/
/* Steps_Metrics.cpp - Metrics step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Metrics(void)
{
  vAddStep("metrics::lockSample"_Stz,   &Step::vMetricsLockSample);
}

/*----------------------------------------------------------------------------*/
/* metrics::lockSample -- sample the lock stats                               */
/*----------------------------------------------------------------------------*/
void Step::vMetricsLockSample(void)
{
  ENTRY();

  StreamDef* pStreamDef = *m_pLogicUnit->m_xpStreamDef;
    
  //InfoSet* pInfoSet = LockMetrics::pSample(pStreamDef);
  
  //m_pDataStack->vPushPtr(pInfoSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
