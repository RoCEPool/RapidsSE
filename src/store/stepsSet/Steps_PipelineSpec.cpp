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
  vAddStep("PipelineSpec[].delete"_Stz,   &Step::vPipelineSpecDelete);
}

/*----------------------------------------------------------------------------*/
/* PipelineSpec[].delete -- delete the pipeline spec                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpecDelete(void)
{
  ENTRY();
  
  LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
  
  if (pLogicUnit->m_xpPipelineSpec.bIsNotNull()) {
    pLogicUnit->m_xpPipelineSpec.vGuardedDeleteComponent(this,(Stz*)__func__);
  }
    
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
