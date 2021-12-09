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
: LifeGuard_t<Pipeline>(this,CN_PIPELINE)
, m_okDebugLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_sPipelineNm   = pPipelineSpec->szPipelineNm();
  
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
  
  vCreateStages();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pipeline::~Pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    
    pStage->vGuardedDelete(CN_STAGE,&pStage,
                           CN_PIPELINE,this,(Stz*)__func__);
  }

  vClear();

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
  
  // get the first stage of the pipeline
  Stage* pStage = pGet(0);
  
  // send cancelMarker to the start Op of the first stage of the pipeline
  pStage->vCancelPipeline(pTriggeringTask);
  
  return true;
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
/* create the stages of this pipeline                                         */
/*----------------------------------------------------------------------------*/
void Pipeline::vCreateStages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  i8 nStages = m_pPipelineSpec->nItems();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pPipelineSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(pStageSpec, this);
    vAppend(pStage);
  }
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
  
  Stage*  pStage0   = pGet(0);
  Stream* pStand0   = pStage0->pGet(0);
  Op*     pOp0      = pStand0->pGet(0);

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Pipeline::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  i8 nOps = 0;
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    nOps += pStage->nOpsOnNode(iNode,m_i_Origin);
  }
  
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
/* get pulsed                                                                 */
/*----------------------------------------------------------------------------*/
bool Pipeline::bPulsedLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_bPulsed;
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
  
  Stage* pStage = pGet(0);
  
  i8 nDivs = pStage->nItems();
  for (i8 i_Stream = 0; i_Stream < nDivs; i_Stream++) {
    Stream* pStream = pStage->pGet(i_Stream);

    i8 nOps = pStream->nItems();
    for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
      Op* pOp = pStream->pGet(i_Op);
      
      if (pOp->szOpNm()->bSame("limitOp"_Stz)) {
        m_bPulsedLimit = true;
        break;
      }
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Stream = 0; i_Stream < nDivs; i_Stream++) {
    Stream* pStream = pStage->pGet(i_Stream);

    i8 nOps = pStream->nItems();
    for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
      Op* pOp = pStream->pGet(i_Op);

      if (pOp->szOpNm()->bSame("scan"_Stz)) {
        m_pOpToPulse = pOp;
        pOp->vPulsedLimit(true);
        break;
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Pipeline::vSpawnOpTasks(i8 iSessionId, i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    
    Stage* pStage = pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSpawnOpTasks(iSessionId, iNode);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* walk                                                                       */
/*----------------------------------------------------------------------------*/
void Pipeline::vWalk(void* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  eCN eClassNo = eGetClassNo();
  
  vTrace("Walk %8s:%ll03d %p\n"_Stz
         
         ,aCN[eClassNo]
         ,(i8)eClassNo
         ,this
         );

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    
    pStage->vWalk(aArg);
  }
}

/*----------------------------------------------------------------------------*/
/* walk                                                                       */
/*----------------------------------------------------------------------------*/
void Pipeline::vWalkLambda(
                 i8 iDepth,
                 std::function<void(Object* pObject, i8 iDepth)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  f_vClosure(this,iDepth);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    
    pStage->vWalkLambda(iDepth+1,f_vClosure);
  }
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
