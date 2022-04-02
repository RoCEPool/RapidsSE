/*============================================================================*/
/* Steps_Pipeline.cpp -- Pipeline step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "VertexSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Marker.hpp"

#include "SP_Pipeline.hpp"
#include "SP_PipeSpec.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PipelineOfOp(void)
{
  vAddStep("[pipeline]"_Stz,                &Step::vPipelineOfOpGet);
  
  vAddStep("[pipeline].addAccum"_Stz,       &Step::vPipelineOfOpAddAccum);
  vAddStep("[pipeline].cancelled@"_Stz,     &Step::vPipelineOfOpCancelledFetch);
  vAddStep("[pipeline].cancelled!"_Stz,     &Step::vPipelineOfOpCancelledStore);
  vAddStep("[pipeline].dropDebugLock"_Stz,  &Step::vPipelineOfOpDropDebugLock);
  vAddStep("[pipeline].earlyExit@"_Stz,     &Step::vPipelineOfOpEarlyExitFetch);
  vAddStep("[pipeline].earlyExit!"_Stz,     &Step::vPipelineOfOpEarlyExitStore);
  vAddStep("[pipeline].errored@"_Stz,       &Step::vPipelineOfOpErroredFetch);
  vAddStep("[pipeline].errored!"_Stz,       &Step::vPipelineOfOpErroredStore);
  vAddStep("[pipeline].getAccum"_Stz,       &Step::vPipelineOfOpGetAccum);
  vAddStep("[pipeline].grabDebugLock"_Stz,  &Step::vPipelineOfOpGrabDebugLock);
  vAddStep("[pipeline].pulsed@"_Stz,        &Step::vPipelineOfOpPulsedFetch);
  vAddStep("[pipeline].pulsed!"_Stz,        &Step::vPipelineOfOpPulsedStore);
}

/*----------------------------------------------------------------------------*/
/* [pipeline].addAccum -- add to an accumulator in this pipeline              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpAddAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].cancelled@ -- has this pipeline been cancelled?                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCancelledFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  m_pDataStack->vPushInt(pPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].cancelled! -- fetch this pipeline's cancelled indicator         */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCancelledStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [pipeline].dropDebugLock -- drop this pipeline's debug lock                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].earlyExit@ -- fetch this pipelines earlyExit indicator          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpEarlyExitFetch(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].earlyExit! -- store this pipeline's earlyExit indicator         */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  pPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].errored@ -- fetch this pipeline errored indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpErroredFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].errored! -- store this pipeline errored indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpErroredStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].get -- get this pipeline                                        */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGet(void)
{
  ENTRY();
  
  if (m_pOp) {
    Pipeline* pPipeline = m_pOp->pPipeline();
    m_pDataStack->vPushPtr(pPipeline);
  }
  else {
    m_pDataStack->vPushPtr(nullptr);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].getAccum -- get an accumulator from this pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGetAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].grabDebugLock -- grab this pipeline's debug lock                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].pulsed@ -- fetch this pipeline's pulsed indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpPulsedFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].pulsed! -- store this pipeline's pulsed indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpPulsedStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
