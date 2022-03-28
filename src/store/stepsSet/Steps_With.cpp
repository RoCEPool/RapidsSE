/*============================================================================*/
/* Steps_With.cpp - with step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_With(void)
{
  vAddStep("[with].subPipeline?"_Stz,  &Step::vWithSubPipeline);
}

/*----------------------------------------------------------------------------*/
/* [with].subPipeline? -- test if compiled with SUBPIPELINE                   */
/*----------------------------------------------------------------------------*/
void Step::vWithSubPipeline(void)
{
  ENTRY();

#ifdef WITH_SUBPIPELINE
    m_pDataStack->vPushInt(true);
#else
    m_pDataStack->vPushInt(false);
#endif
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
