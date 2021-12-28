/*============================================================================*/
/* Steps_IntStack.cpp - intStack step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


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
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.delete -- delete intStack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntStackDelete(void)
{
  ENTRY();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.items@ -- get nItems currently on given int stack                 */
/*----------------------------------------------------------------------------*/
void Step::vIntStackItemsFetch(void)
{
  ENTRY();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.pop -- pop int from stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntStackPop(void)
{
  ENTRY();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IntStack.push -- push int onto stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vIntStackPush(void)
{
  ENTRY();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
