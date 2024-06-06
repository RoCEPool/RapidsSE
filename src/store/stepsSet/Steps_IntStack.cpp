/*============================================================================*/
/* Steps_IntStack.cpp - intStack step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "IntStack.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IntStack(void)
{
  vAddStep("IntStack::new"_Stz,    &Step::vIntStackNew);
  
  vAddStep("IntStack.delete"_Stz,  &Step::vIntStackDelete);
  vAddStep("IntStack.items@"_Stz,  &Step::vIntStackItemsFetch);
  vAddStep("IntStack.pop"_Stz,     &Step::vIntStackPop);
  vAddStep("IntStack.push"_Stz,    &Step::vIntStackPush);
}

/*----------------------------------------------------------------------------*/
/* IntStack::new -- dew an intStack                                           */
/*----------------------------------------------------------------------------*/
void Step::vIntStackNew(void)
{
  ENTRY();
  
  IntStack* pIntStack = new IntStack;
  
  m_oDataStack.vPushPtr(pIntStack);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.delete -- delete intStack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntStackDelete(void)
{
  ENTRY();
  
  IntStack* pIntStack = _pIntStack(m_oDataStack.pPopPtr(__func__));
  
  delete pIntStack;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.items@ -- get nItems currently on given int stack                 */
/*----------------------------------------------------------------------------*/
void Step::vIntStackItemsFetch(void)
{
  ENTRY();
  
  IntStack* pIntStack = _pIntStack(m_oDataStack.pPopPtr(__func__));

  i8 nItems = pIntStack->nItems();
  
  m_oDataStack.vPushInt(nItems);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.pop -- pop int from stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntStackPop(void)
{
  ENTRY();
  
  IntStack* pIntStack = _pIntStack(m_oDataStack.pPopPtr(__func__));

  i8 iInt = pIntStack->iPop();
  
  m_oDataStack.vPushInt(iInt);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.push -- push int onto stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vIntStackPush(void)
{
  ENTRY();
  
  i8 iInt = m_oDataStack.iPopInt(__func__);

  IntStack* pIntStack = _pIntStack(m_oDataStack.pPopPtr(__func__));

  pIntStack->vPush(iInt);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
