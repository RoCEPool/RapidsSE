/*============================================================================*/
/* Steps_RespondToOp.cpp -- Operator to which responses are directed stepGroup*/
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RespondToOp(void)
{
  //vAddStep("[pipeline]"_Stz,                &Step::vPipelineOfOpGet);
  
}

/*----------------------------------------------------------------------------*/
/* [pipeline].get -- get this pipeline                                        */
/*----------------------------------------------------------------------------*/
//void Step::vPipelineOfOpGet(void)
//{
//  ENTRY();
//
//  if (m_pOp) {
//    Pipeline* pPipeline = m_pOp->pPipeline();
//    m_pDataStack->vPushPtr(pPipeline);
//  }
//  else {
//    m_pDataStack->vPushPtr(nullptr);
//  }
//
//  EXIT();
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
