/*============================================================================*/
/* Steps_Steps.cpp - Steps step group                               */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Steps(void)
{
  vAddStep("Steps::reset"_Stz, &Step::vStepsReset);
  vAddStep("Steps::start"_Stz, &Step::vStepsStart);
  vAddStep("Steps::write"_Stz, &Step::vStepsWrite);
}

/*----------------------------------------------------------------------------*/
/* Steps::reset -- reset steps                                           */
/*----------------------------------------------------------------------------*/
void Step::vStepsReset(void)
{
  ENTRY();
  
#ifndef WITH_STEP_TIMING
  
  vTrace("WARNING: WITH_STEP_TIMING not defined\n"_Stz);
  return;
  
#else
  
  if (AM::pSteps())
    AM::pSteps()->vReset();
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Steps::start -- start step tracking                                   */
/*----------------------------------------------------------------------------*/
void Step::vStepsStart(void)
{
  ENTRY();
  
#ifndef WITH_STEP_TIMING
  
  vTrace("WARNING: WITH_STEP_TIMING not defined\n"_Stz);
  return;
  
#else
  
  if (! AM::pSteps())
    AM::vSteps(new Steps);
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Steps::write -- write steps                                          */
/*----------------------------------------------------------------------------*/
void Step::vStepsWrite(void)
{
  ENTRY();

#ifndef WITH_STEP_TIMING
  
  vTrace("WARNING: WITH_STEP_TIMING not defined\n"_Stz);
  return;
  
#else

  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
  
  if (AM::pSteps())
    AM::pSteps()->vWrite(sPath);
  
#endif
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
