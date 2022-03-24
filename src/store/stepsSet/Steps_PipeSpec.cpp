/*============================================================================*/
/* Steps_PipeSpec.cpp -- PipeSpec step group                                  */
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
#include "PipeSpec.hpp"
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
void Step::vAddSteps_PipeSpec(void)
{
  vAddStep("PipeSpec[]"_Stz,              &Step::vPipeSpecGet);
  vAddStep("PipeSpec[].delete"_Stz,       &Step::vPipeSpecDelete);
  vAddStep("PipeSpec[].dump"_Stz,         &Step::vPipeSpecDump);
  vAddStep("PipeSpec[].get"_Stz,          &Step::vPipeSpecGet);
  vAddStep("PipeSpec[].optimisable"_Stz,  &Step::vPipeSpecOptimisable);
  vAddStep("PipeSpec[].set"_Stz,          &Step::vPipeSpecSet);
}

/*----------------------------------------------------------------------------*/
/* PipeSpec[].delete -- delete the pipeline spec                              */
/*----------------------------------------------------------------------------*/
void Step::vPipeSpecDelete(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpPipeSpec.bIsNotNull()) {
    m_pLogicUnit
      ->m_xpPipeSpec.vGuardedDeleteComponent(CN_PIPESPEC,
                                                 CN_STEP,this,(Stz*)__func__);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeSpec[].dump -- dump the pipeline spec                                  */
/*----------------------------------------------------------------------------*/
void Step::vPipeSpecDump(void)
{
  ENTRY();
  

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeSpec[].get -- get pipeline spec                                        */
/*----------------------------------------------------------------------------*/
void Step::vPipeSpecGet(void)
{
  ENTRY();

  if (m_pLogicUnit->m_xpPipeSpec.bIsNotNull())
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpPipeSpec);
  else
    m_pDataStack->vPushPtr(nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeSpec[].optimisable -- is p.spec optimisable / to be optimised?         */
/*----------------------------------------------------------------------------*/
void Step::vPipeSpecOptimisable(void)
{
  ENTRY();
  
  bool bOptimisable = false;
  
  PipeSpec* pPipeSpec = (* m_pLogicUnit->m_xpPipeSpec);
  
  
  
  Job* pJob = MY::pJob();
  
  Stz* szPipelineNm = pPipeSpec->szPipelineNm();
  
  i8 iOptimiserLvl = AM::SYSARGS::iOptimiserLvl();

  if (   ! iOptimiserLvl
      || ! pJob->bIsOriginator()
      ||   szPipelineNm->bSame("sqlCompiler"_Stz)
      || ! pPipeSpec->pJobTags()->bHasName(C_TAG_OPTIMISE)
      ||   pPipeSpec->pJobTags()->bHasName(C_TAG_OPTIMISED)
      )
    bOptimisable = false;
  else
    bOptimisable = true;
  
  
  m_pDataStack->vPushInt(bOptimisable);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeSpec[].set -- set pipeSpec                                             */
/*----------------------------------------------------------------------------*/
void Step::vPipeSpecSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPipeSpec =
    _pPipeSpec(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
