/*============================================================================*/
/* Steps_Expressions.cpp - Expressions step group                                     */
/*============================================================================*/

#include "STORE.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Expressions(void)
{
  vAddStep("Expressions::lockSample"_Stz,   &Step::vExpressionsLockSample);
}

/*----------------------------------------------------------------------------*/
/* Expressions::lockSample -- sample the lock stats                               */
/*----------------------------------------------------------------------------*/
void Step::vExpressionsLockSample(void)
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
