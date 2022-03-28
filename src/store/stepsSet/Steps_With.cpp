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
  vAddStep("[with].2pcSimple?"_Stz,    &Step::vWith2pcSimple);
  vAddStep("[with].subPipeline?"_Stz,  &Step::vWithSubPipeline);
}

/*----------------------------------------------------------------------------*/
/* [with].2pcSimple? -- test if compiled with 2PC+SIMPLE                      */
/*----------------------------------------------------------------------------*/
void Step::vWith2pcSimple(void)
{
  ENTRY();

#ifdef WITH_2PC_SIMPLE
    m_pDataStack->vPushInt(true);
#else
    m_pDataStack->vPushInt(false);
#endif
  
  EXIT();
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
