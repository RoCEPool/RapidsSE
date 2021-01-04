/*============================================================================*/
/* Steps_RegNow.cpp - now[] step group                                        */
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

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegNow(void)
{
  vAddStep("now[]"_Stz,      &Step::vRegNowGet);
}

/*----------------------------------------------------------------------------*/
/* now[].get -- get now                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegNowGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpNow);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
