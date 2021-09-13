/*============================================================================*/
/* Steps_PipelineSpec.cpp -- PipelineSpec step group                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "PipelineSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Marker.hpp"

#include "SP_Pipeline.hpp"
#include "SP_PipelineSpec.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PipelineSpec(void)
{
  vAddStep("PipelineSpec[]"_Stz,          &Step::vPipelineSpecGet);
  vAddStep("PipelineSpec[].delete"_Stz,   &Step::vPipelineSpecDelete);
  vAddStep("PipelineSpec[].get"_Stz,      &Step::vPipelineSpecGet);
}

/*----------------------------------------------------------------------------*/
/* PipelineSpec[].delete -- delete the pipeline spec                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpecDelete(void)
{
  ENTRY();
   
  if (m_p_TaskLogicUnit->m_xpPipelineSpec.bIsNotNull()) {
    m_p_TaskLogicUnit
      ->m_xpPipelineSpec.vGuardedDeleteComponent(CN_PIPELINESPEC,
                                                 CN_STEP,this,(Stz*)__func__);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipelineSpec[].get -- get pipeline spec                                    */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpecGet(void)
{
  ENTRY();

  if (m_p_TaskLogicUnit->m_xpPipelineSpec.bIsNotNull())
    m_pDataStack->vPushPtr(* m_p_TaskLogicUnit->m_xpPipelineSpec);
  else
    m_pDataStack->vPushPtr(nullptr);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
