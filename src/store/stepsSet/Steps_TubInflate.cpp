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
void Step::vAddSteps_Inflator(void)
{
  vAddStep("Inflator[].crwInflate"_Stz,    &Step::vInflatorCrwInflate);
  vAddStep("Inflator[]"_Stz,               &Step::vInflatorGet);
  vAddStep("Inflator[].delete"_Stz,        &Step::vInflatorDelete);
  vAddStep("Inflator[].new"_Stz,           &Step::vInflatorNew);
}

/*----------------------------------------------------------------------------*/
/* Inflator[].crwInflate -- inflate a crw buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vInflatorCrwInflate(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Inflator[].delete -- delete the inflator                                   */
/*----------------------------------------------------------------------------*/
void Step::vInflatorDelete(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpInflator.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Inflator[].get -- get the inflator                                         */
/*----------------------------------------------------------------------------*/
void Step::vInflatorGet(void)
{
  ENTRY();
  
  Inflator* pInflator = * m_p_TaskLogicUnit->m_xpInflator;
  
  m_pDataStack->vPushPtr(pInflator);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Inflator[].new -- new an inflator                                          */
/*----------------------------------------------------------------------------*/
void Step::vInflatorNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpInflator = new Inflator;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
