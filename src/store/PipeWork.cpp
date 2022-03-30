/*============================================================================*/
/* PipeWork.cpp -- PipeWork                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "StageSet.hpp"
#include "PipeWork.hpp"
#include "PipeSpec.hpp"
#include "StageSpec.hpp"
#include "Pipeline.hpp"
#include "SubPipeWorks.hpp"

#include "SP_PipeWork.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PipeWork,CN_PIPEWORK)
__aNewFunction(PipeWork,CN_PIPEWORK)
__vDeleteFunction(PipeWork,CN_PIPEWORK)
__aFooterFunction(PipeWork)




#ifdef WITH_STAGESET

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipeWork::PipeWork(Pipeline* pPipeline, PipeSpec* pPipeSpec)
: ListNL_t<PipeWork>(CN_PIPEWORK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  m_pPipeline = pPipeline;
  m_pPipeSpec = pPipeSpec;

  m_pStageSet = new StageSet;

  _checkInitialised_(CN_PIPEWORK);
  
  vCreatePipework();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipeWork::~PipeWork(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  while (PipeWork *pSupPipeWork = pPop())
    delete pSupPipeWork;
  
  delete m_pStageSet;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* cancel the pipeline                                                        */
/*----------------------------------------------------------------------------*/
bool PipeWork::bCancel(Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pStageSet->pGet(0);
  
  // send cancelMarker to the start Op of the first stage of the pipeline
  pStage->vCancelPipeline(pTriggeringTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get formwork spec                                                          */
/*----------------------------------------------------------------------------*/
PipeSpec* PipeWork::pPipeSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  return m_pPipeSpec;
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* PipeWork::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage*  pStage0 = m_pStageSet->pGet(0);
  Op*     pOp0    = pStage0->pOp0();

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 PipeWork::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  i8 nOps = 0;
  
  i8 iOriginNodeNo = m_pPipeline->iOriginNodeNo();
  
  i8 nStages = this->nStages();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = m_pStageSet->pGet(i_Stage);
    nOps += pStage->nOpsOnNode(iNode,iOriginNodeNo);
  }
  
  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* PipeWork::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void PipeWork::vSetInvoker(Task* pInvoker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nStages(); i_Stage++) {
    
    Stage* pStage = m_pStageSet->pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSetInvoker(pInvoker);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void PipeWork::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage* pStage = m_pStageSet->pGet(0);
  
  i8 nDivs = pStage->nItems();
  for (i8 i_Stream = 0; i_Stream < nDivs; i_Stream++) {
    Stream* pStream = pStage->pGet(i_Stream);

    i8 nOps = pStream->nItems();
    for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
      Op* pOp = pStream->pGet(i_Op);
      
      if (pOp->szOpNm()->bSame("limitOp"_Stz)) {
        m_pPipeline->vPulsedLimit(true);
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
        m_pPipeline->vOpToPulse(pOp);
        pOp->vPulsedLimit(true);
        break;
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void PipeWork::vSpawnOpTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nStages(); i_Stage++) {
    
    Stage* pStage = m_pStageSet->pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSpawnOpTasks();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get stage 0                                                                */
/*----------------------------------------------------------------------------*/
Stage* PipeWork::pStage0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage*  pStage0 = m_pStageSet->pGet(0);

  return pStage0;
}

/*----------------------------------------------------------------------------*/
/* get number of stages                                                       */
/*----------------------------------------------------------------------------*/
i8 PipeWork::nStages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  i8 nStages = m_pStageSet->nItems();
  
  return nStages;
}

/*----------------------------------------------------------------------------*/
/* get stage set                                                              */
/*----------------------------------------------------------------------------*/
StageSet* PipeWork::pStageSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  return m_pStageSet;
}

/*----------------------------------------------------------------------------*/
/* get stream 0                                                               */
/*----------------------------------------------------------------------------*/
Stream* PipeWork::pStream0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage*  pStage0  = this->pStage0();
  Stream* pStream0 = pStage0->pStream0();

  return pStream0;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the formwork for pipeline                                           */
/*----------------------------------------------------------------------------*/

void PipeWork::vCreatePipework(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);

  i8 nStages = m_pPipeSpec->nStageSpecs();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pPipeSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(pStageSpec, this);
    m_pStageSet->vAppend(pStage);
    pStage->vCreateStreams();
  }
}

//##############################################################################
//##############################################################################
//##############################################################################

#else

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipeWork::PipeWork(Pipeline* pPipeline, PipeSpec* pPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  m_pPipeline = pPipeline;
  m_pPipeSpec = pPipeSpec;

  m_pSubPipeWorks = nullptr;
  m_pStageSet = nullptr;

  _checkInitialised_(CN_PIPEWORK);
  
  vCreatePipework();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipeWork::~PipeWork(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  delete m_pSubPipeWorks;

  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    
    delete pStage;
  }

  vClear();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* cancel the pipeline                                                        */
/*----------------------------------------------------------------------------*/
bool PipeWork::bCancel(Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = pGet(0);
  
  // send cancelMarker to the start Op of the first stage of the pipeline
  pStage->vCancelPipeline(pTriggeringTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get formwork spec                                                          */
/*----------------------------------------------------------------------------*/
PipeSpec* PipeWork::pPipeSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  return m_pPipeSpec;
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* PipeWork::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage*  pStage0 = pGet(0);
  Op*     pOp0    = pStage0->pOp0();

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 PipeWork::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  i8 nOps = 0;
  
  i8 iOriginNodeNo = m_pPipeline->iOriginNodeNo();
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    nOps += pStage->nOpsOnNode(iNode,iOriginNodeNo);
  }
  
  return nOps;
}
/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* PipeWork::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void PipeWork::vSetInvoker(Task* pInvoker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    
    Stage* pStage = pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSetInvoker(pInvoker);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void PipeWork::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage* pStage = pGet(0);
  
  i8 nDivs = pStage->nItems();
  for (i8 i_Stream = 0; i_Stream < nDivs; i_Stream++) {
    Stream* pStream = pStage->pGet(i_Stream);

    i8 nOps = pStream->nItems();
    for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
      Op* pOp = pStream->pGet(i_Op);
      
      if (pOp->szOpNm()->bSame("limitOp"_Stz)) {
        m_pPipeline->vPulsedLimit(true);
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
        m_pPipeline->vOpToPulse(pOp);
        pOp->vPulsedLimit(true);
        break;
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void PipeWork::vSpawnOpTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    
    Stage* pStage = pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSpawnOpTasks();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get stage 0                                                                */
/*----------------------------------------------------------------------------*/
Stage* PipeWork::pStage0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage*  pStage0 = pGet(0);

  return pStage0;
}

/*----------------------------------------------------------------------------*/
/* get stage set                                                              */
/*----------------------------------------------------------------------------*/
StageSet* PipeWork::pStageSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  return m_pStageSet;
}

/*----------------------------------------------------------------------------*/
/* get stream 0                                                               */
/*----------------------------------------------------------------------------*/
Stream* PipeWork::pStream0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  Stage*  pStage0  = this->pStage0();
  Stream* pStream0 = pStage0->pStream0();

  return pStream0;
}

/*----------------------------------------------------------------------------*/
/* get sub-pipe works                                                         */
/*----------------------------------------------------------------------------*/
SubPipeWorks* PipeWork::pSubPipeWorks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);
  
  return m_pSubPipeWorks;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the formwork for pipeline                                           */
/*----------------------------------------------------------------------------*/
void PipeWork::vCreatePipework(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEWORK,_FOOTER_);

  i8 nStages = m_pPipeSpec->nItems();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pPipeSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(pStageSpec, this);
    vAppend(pStage);
    pStage->vCreateStreams();
  }
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
