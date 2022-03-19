/*============================================================================*/
/* Pipeline.cpp -- Pipeline                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipework.hpp"
#include "Pipeline.hpp"
#include "PipelineSpec.hpp"
#include "StageSpec.hpp"

#include "SP_Pipeline.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Pipeline,CN_PIPELINE)
__aNewFunction(Pipeline,CN_PIPELINE)
__vDeleteFunction(Pipeline,CN_PIPELINE)
__aFooterFunction(Pipeline)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pipeline::Pipeline(Job* pPipelineJob, PipelineSpec* pPipelineSpec, i8 i_Origin)
: Object(CN_PIPELINE)
, LifeGuard_t<Pipeline>(this,CN_PIPELINE)
, m_okDebugLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_sPipelineNm   = pPipelineSpec->szPipelineNm();
  
  m_pFormWork     = nullptr;
  
  m_pPipelineJob  = pPipelineJob;
  m_pPipelineSpec = pPipelineSpec;
  m_i_Origin      = i_Origin;

  m_bEarlyExit    = false;
  m_bErrored      = false;
  m_bPulsed       = false;
  m_bPulsedLimit  = false;
  m_pOpToPulse    = nullptr;
  
  m_bCancelled    = false;
    
  m_ixAccum0 = 0;
  m_ixAccum1 = 0;
  m_ixAccum2 = 0;

  _checkInitialised_(CN_PIPELINE);
  
  m_pFormWork = new Pipework(this,pPipelineSpec->pPipeworkSpec());
  //vCreatePipework();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pipeline::~Pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_pFormWork->vGuardedDelete(CN_PIPEWORK,&m_pFormWork,
                              CN_PIPELINE,this,(Stz*)__func__);
  
  m_pPipelineSpec->vGuardedDelete(CN_PIPELINESPEC,&m_pPipelineSpec,
                                  CN_PIPELINE,this,(Stz*)__func__);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add to accumumlator                                                         */
/*----------------------------------------------------------------------------*/
void Pipeline::vAddAccum(i8 i_Accum, i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  switch (i_Accum) {
    case 0: m_ixAccum0.iAdd(iValue); break;
    case 1: m_ixAccum1.iAdd(iValue); break;
    case 2: m_ixAccum2.iAdd(iValue); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* cancel the pipeline                                                        */
/*----------------------------------------------------------------------------*/
bool Pipeline::bCancel(Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  // check if pipeline has already been pusled
  if (m_bPulsed)
    return false;
  
  bool bCancelled = m_pFormWork->bCancel(pTriggeringTask);
  
  return bCancelled;
}

/*----------------------------------------------------------------------------*/
/* pipeline has been  cancelled                                               */
/*----------------------------------------------------------------------------*/
void Pipeline::vCancelled(bool bCancelled)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bCancelled = bCancelled;
}

/*----------------------------------------------------------------------------*/
/* is pipeline being cancelled                                                */
/*----------------------------------------------------------------------------*/
bool Pipeline::bCancelled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  if (m_bCancelled)
    vDebugPoint();
  
  return m_bCancelled;
}

/*----------------------------------------------------------------------------*/
/* drop the debug lock                                                        */
/*----------------------------------------------------------------------------*/
void Pipeline::vDropDebugLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_okDebugLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* set early exit                                                             */
/*----------------------------------------------------------------------------*/
void Pipeline::vEarlyExit(bool bEarlyExit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  m_bEarlyExit = bEarlyExit;
}

/*----------------------------------------------------------------------------*/
/* get early exit                                                             */
/*----------------------------------------------------------------------------*/
bool Pipeline::bEarlyExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  return m_bEarlyExit;
}

/*----------------------------------------------------------------------------*/
/* set errored                                                                */
/*----------------------------------------------------------------------------*/
void Pipeline::vErrored(bool bErrored)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bErrored = bErrored;
}

/*----------------------------------------------------------------------------*/
/* get errored                                                                 */
/*----------------------------------------------------------------------------*/
bool Pipeline::bErrored(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_bErrored;
}

/*----------------------------------------------------------------------------*/
/* get formwork                                                               */
/*----------------------------------------------------------------------------*/
Pipework* Pipeline::pPipework(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pFormWork;
}

/*----------------------------------------------------------------------------*/
/* get accumumlator                                                           */
/*----------------------------------------------------------------------------*/
i8 Pipeline::iGetAccum(i8 i_Accum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  i8 iValue = -1;

  switch (i_Accum) {
    case 0: iValue = m_ixAccum0.iVal(); break;
    case 1: iValue = m_ixAccum1.iVal(); break;
    case 2: iValue = m_ixAccum2.iVal(); break;
    default:
      NEVER_REACH;
  }
  
  return iValue;
}

/*----------------------------------------------------------------------------*/
/* grab the debug lock                                                        */
/*----------------------------------------------------------------------------*/
void Pipeline::vGrabDebugLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_okDebugLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* get the originating node                                                   */
/*----------------------------------------------------------------------------*/
i8 Pipeline::i_Origin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_i_Origin;
}

/*----------------------------------------------------------------------------*/
/* get the job                                                                */
/*----------------------------------------------------------------------------*/
Job* Pipeline::pPipelineJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pPipelineJob;
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* Pipeline::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  Op* pOp0 = m_pFormWork->pOp0();

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Pipeline::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  i8 nOps = m_pFormWork->nOpsOnNode(iNode);
    
  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get pOpToPulse                                                             */
/*----------------------------------------------------------------------------*/
Op* Pipeline::pOpToPulse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pOpToPulse;
}

/*----------------------------------------------------------------------------*/
/* set pOpToPulse                                                             */
/*----------------------------------------------------------------------------*/
void Pipeline::vOpToPulse(Op* pOpToPulse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_pOpToPulse = pOpToPulse;
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz* Pipeline::szPipelineNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_sPipelineNm;
}

/*----------------------------------------------------------------------------*/
/* get pipeline spec                                                          */
/*----------------------------------------------------------------------------*/
PipelineSpec* Pipeline::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* get bPulsedLimit?                                                          */
/*----------------------------------------------------------------------------*/
bool Pipeline::bPulsedLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_bPulsedLimit;
}

/*----------------------------------------------------------------------------*/
/* set bPulsedLimit?                                                          */
/*----------------------------------------------------------------------------*/
void Pipeline::vPulsedLimit(bool bPulsedLimit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bPulsedLimit = bPulsedLimit;
}

/*----------------------------------------------------------------------------*/
/* set pulsed                                                                 */
/*----------------------------------------------------------------------------*/
void Pipeline::vPulsed(bool bPulsed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bPulsed = bPulsed;
}

/*----------------------------------------------------------------------------*/
/* get pulsed                                                                 */
/*----------------------------------------------------------------------------*/
bool Pipeline::bPulsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_bPulsed;
}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void Pipeline::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_pFormWork->vSetupJob();
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Pipeline::vSpawnOpTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  m_pFormWork->vSpawnOpTasks();
}

/*----------------------------------------------------------------------------*/
/* get stage 0                                                                */
/*----------------------------------------------------------------------------*/
Stage* Pipeline::pStage0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  Stage* pStage0 = m_pFormWork->pStage0();

  return pStage0;
}

/*----------------------------------------------------------------------------*/
/* get stream 0                                                               */
/*----------------------------------------------------------------------------*/
Stream* Pipeline::pStream0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  Stream* pStream0 = m_pFormWork->pStream0();

  return pStream0;
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the formwork for this pipeline                                      */
/*----------------------------------------------------------------------------*/
void Pipeline::vCreatePipework(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  m_pFormWork->vCreatePipework();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
