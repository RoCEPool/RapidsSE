/*============================================================================*/
/* Steps_Tub.cpp - Working tub of buffers                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TubInflate(void)
{
  vAddStep("tubInflate[].crwInflate"_Stz,    &Step::vTubInflateCrwInflate);
  vAddStep("tubInflate[]"_Stz,               &Step::vTubInflateGet);
  vAddStep("tubInflate[].delete"_Stz,        &Step::vTubInflateDelete);
  vAddStep("tubInflate[].new"_Stz,           &Step::vTubInflateNew);
}

/*----------------------------------------------------------------------------*/
/* tubInflate[].crwInflate -- inflate a crw buffer                            */
/*----------------------------------------------------------------------------*/
void Step::vTubInflateCrwInflate(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tubInflate[].delete -- delete the tub inflate                              */
/*----------------------------------------------------------------------------*/
void Step::vTubInflateDelete(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpTubInflate.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tubInflate[].get -- get the tub inflate                                    */
/*----------------------------------------------------------------------------*/
void Step::vTubInflateGet(void)
{
  ENTRY();
  
  TubInflate* pTubInflate = * m_p_TaskLogicUnit->m_xpTubInflate;
  
  m_pDataStack->vPushPtr(pTubInflate);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tubInflate[].new -- new a tub inflate                                      */
/*----------------------------------------------------------------------------*/
void Step::vTubInflateNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpTubInflate = new TubInflate;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
