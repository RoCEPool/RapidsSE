/*============================================================================*/
/* Steps_Supervisor.cpp -- Supervisor step group                              */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Supervisor(void)
{
  vAddStep("Supervisor::collectInfo"_Stz,  &Step::vSupervisorCollectInfo);
}

/*----------------------------------------------------------------------------*/
/* Supervisor::collectInfo -- collect heartbeat related info                  */
/*----------------------------------------------------------------------------*/
void Step::vSupervisorCollectInfo(void)
{
  ENTRY();
  
  //vTrace("  ** Step::vSupervisorCollectInfo ** \n"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
