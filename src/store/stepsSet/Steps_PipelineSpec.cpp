/*============================================================================*/
/* Steps_PipelineSpec.cpp -- PipelineSpec step group                          */
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
  vAddStep("PipelineSpec[]"_Stz,              &Step::vPipelineSpecGet);
  vAddStep("PipelineSpec[].delete"_Stz,       &Step::vPipelineSpecDelete);
  vAddStep("PipelineSpec[].get"_Stz,          &Step::vPipelineSpecGet);
  vAddStep("PipelineSpec[].optimisable"_Stz,  &Step::vPipelineSpecOptimisable);
  vAddStep("PipelineSpec[].set"_Stz,          &Step::vPipelineSpecSet);
}

/*----------------------------------------------------------------------------*/
/* PipelineSpec[].delete -- delete the pipeline spec                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpecDelete(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpPipelineSpec.bIsNotNull()) {
    m_pLogicUnit
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

  if (m_pLogicUnit->m_xpPipelineSpec.bIsNotNull())
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpPipelineSpec);
  else
    m_pDataStack->vPushPtr(nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipelineSpec[].optimisable -- is p.spec optimisable / to be optimised?     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpecOptimisable(void)
{
  ENTRY();
  
  bool bOptimisable = false;
  
  PipelineSpec* pPipelineSpec = (* m_pLogicUnit->m_xpPipelineSpec);
  
  
  
  Job* pJob = MY::pJob();
  
  Stz* szPipelineNm = pPipelineSpec->szPipelineNm();
  
  i8 iOptimiserLvl = AM::SYSARGS::iOptimiserLvl();

  if (   ! iOptimiserLvl
      || ! pJob->bIsOriginator()
      ||   szPipelineNm->bSame("sqlCompiler"_Stz)
      || ! pPipelineSpec->pJobTags()->bHasName(C_TAG_OPTIMISE)
      ||   pPipelineSpec->pJobTags()->bHasName(C_TAG_OPTIMISED)
      )
    bOptimisable = false;
  else
    bOptimisable = true;
  
  
  m_pDataStack->vPushInt(bOptimisable);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipelineSpec[].set -- set pipelineSpec                                     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpecSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPipelineSpec =
    _pPipelineSpec(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
