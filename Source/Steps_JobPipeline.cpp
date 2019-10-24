/*============================================================================*/
/* Steps_JobPipeline.cpp - JobPipeline step group                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPipeline(void)
{
  vAddStep("Job_Pipeline.begin"_Stz,    &Step::vJob_PipelineBegin);
  vAddStep("Job_Pipeline.cancel"_Stz,   &Step::vJob_PipelineCancel);
  vAddStep("Job_Pipeline.end"_Stz,      &Step::vJob_PipelineEnd);
  vAddStep("Job_Pipeline.execute"_Stz,  &Step::vJob_PipelineExecute);
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.begin -- begin the Pipeline job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineBegin(void)
{
  ENTRY("vJob_PipelineBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJobPtr);
  
  pJob_Pipeline->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.cancel -- cancel the Pipeline job                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCancel(void)
{
  ENTRY("vJob_PipelineCancel"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJobPtr);
  
  pJob_Pipeline->vCancel();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.end -- end the Pipeline job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineEnd(void)
{
  ENTRY("vJob_PipelineEnd"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJobPtr);
  
  pJob_Pipeline->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.execute -- execute                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineExecute(void)
{
  ENTRY("vJob_PipelineExecute"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJobPtr);
  
  pJob_Pipeline->vExecute();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
