/*============================================================================*/
/* Formwork.cpp -- Formwork                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Formwork.hpp"
#include "FormworkSpec.hpp"
#include "StageSpec.hpp"
#include "Pipeline.hpp"

#include "SP_Formwork.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Formwork,CN_FORMWORK)
__aNewFunction(Formwork,CN_FORMWORK)
__vDeleteFunction(Formwork,CN_FORMWORK)
__aFooterFunction(Formwork)


#ifdef WITH_FORMWORK

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Formwork::Formwork(Pipeline* pPipeline, FormworkSpec* pFormworkSpec)
: LifeGuard_t<Formwork>(this,CN_FORMWORK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  m_pPipeline     = pPipeline;
  m_pFormworkSpec = pFormworkSpec;

  _checkInitialised_(CN_FORMWORK);
  
  vCreateFormwork();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Formwork::~Formwork(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    
    pStage->vGuardedDelete(CN_STAGE,&pStage,
                           CN_FORMWORK,this,(Stz*)__func__);
  }

  vClear();

//  m_pFormworkSpec->vGuardedDelete(CN_FORMWORKSPEC,&m_pFormworkSpec,
//                                  CN_FORMWORK,this,(Stz*)__func__);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* cancel the pipeline                                                        */
/*----------------------------------------------------------------------------*/
bool Formwork::bCancel(Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = pGet(0);
  
  // send cancelMarker to the start Op of the first stage of the pipeline
  pStage->vCancelPipeline(pTriggeringTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get formwork spec                                                          */
/*----------------------------------------------------------------------------*/
FormworkSpec* Formwork::pFormworkSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_pFormworkSpec;
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* Formwork::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  Stage*  pStage0   = pGet(0);
  Stream* pStand0   = pStage0->pGet(0);
  Op*     pOp0      = pStand0->pGet(0);

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Formwork::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  i8 nOps = 0;
  
  i8 i_Origin = m_pPipeline->i_Origin();
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    nOps += pStage->nOpsOnNode(iNode,i_Origin);
  }
  
  return nOps;
}
/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Formwork::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void Formwork::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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
void Formwork::vSpawnOpTasks(i8 iSessionId, i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    
    Stage* pStage = pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSpawnOpTasks(iSessionId, iNode);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the formwork for pipeline                                           */
/*----------------------------------------------------------------------------*/
void Formwork::vCreateFormwork(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);

  i8 nStages = m_pFormworkSpec->nItems();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pFormworkSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(pStageSpec, this);
    vAppend(pStage);
    pStage->vCreateStreams();
  }
}


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#else

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Formwork::Formwork(Job* pFormworkJob, FormworkSpec* pFormworkSpec, i8 i_Origin)
: LifeGuard_t<Formwork>(this,CN_FORMWORK)
, m_okDebugLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  m_sFormworkNm   = pFormworkSpec->szPipelineNm();
  
  m_pFormworkJob  = pFormworkJob;
  m_pFormworkSpec = pFormworkSpec;
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

  _checkInitialised_(CN_FORMWORK);
  
  vCreateStages();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Formwork::~Formwork(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    
    pStage->vGuardedDelete(CN_STAGE,&pStage,
                           CN_FORMWORK,this,(Stz*)__func__);
  }

  vClear();

  m_pFormworkSpec->vGuardedDelete(CN_FORMWORKSPEC,&m_pFormworkSpec,
                                  CN_FORMWORK,this,(Stz*)__func__);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add to accumumlator                                                         */
/*----------------------------------------------------------------------------*/
void Formwork::vAddAccum(i8 i_Accum, i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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
bool Formwork::bCancel(Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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
void Formwork::vCancelled(bool bCancelled)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  m_bCancelled = bCancelled;
}

/*----------------------------------------------------------------------------*/
/* is pipeline being cancelled                                                */
/*----------------------------------------------------------------------------*/
bool Formwork::bCancelled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  if (m_bCancelled)
    vDebugPoint();
  
  return m_bCancelled;
}

/*----------------------------------------------------------------------------*/
/* drop the debug lock                                                        */
/*----------------------------------------------------------------------------*/
void Formwork::vDropDebugLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  m_okDebugLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* create the stages of this pipeline                                         */
/*----------------------------------------------------------------------------*/
void Formwork::vCreateStages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);

  i8 nStages = m_pFormworkSpec->nItems();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pFormworkSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(pStageSpec,nullptr);// this);
    vAppend(pStage);
    pStage->vCreateStreams();
  }
}

/*----------------------------------------------------------------------------*/
/* set early exit                                                             */
/*----------------------------------------------------------------------------*/
void Formwork::vEarlyExit(bool bEarlyExit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);

  m_bEarlyExit = bEarlyExit;
}

/*----------------------------------------------------------------------------*/
/* get early exit                                                             */
/*----------------------------------------------------------------------------*/
bool Formwork::bEarlyExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);

  return m_bEarlyExit;
}

/*----------------------------------------------------------------------------*/
/* set errored                                                                */
/*----------------------------------------------------------------------------*/
void Formwork::vErrored(bool bErrored)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  m_bErrored = bErrored;
}

/*----------------------------------------------------------------------------*/
/* get errored                                                                 */
/*----------------------------------------------------------------------------*/
bool Formwork::bErrored(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_bErrored;
}

/*----------------------------------------------------------------------------*/
/* get accumumlator                                                           */
/*----------------------------------------------------------------------------*/
i8 Formwork::iGetAccum(i8 i_Accum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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
void Formwork::vGrabDebugLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  m_okDebugLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* get the originating node                                                   */
/*----------------------------------------------------------------------------*/
i8 Formwork::i_Origin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_i_Origin;
}

/*----------------------------------------------------------------------------*/
/* get the job                                                                */
/*----------------------------------------------------------------------------*/
Job* Formwork::pFormworkJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_pFormworkJob;
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* Formwork::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  Stage*  pStage0   = pGet(0);
  Stream* pStand0   = pStage0->pGet(0);
  Op*     pOp0      = pStand0->pGet(0);

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Formwork::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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
Op* Formwork::pOpToPulse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_pOpToPulse;
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz* Formwork::szFormworkNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_sFormworkNm;
}

/*----------------------------------------------------------------------------*/
/* get pipeline spec                                                          */
/*----------------------------------------------------------------------------*/
FormworkSpec* Formwork::pFormworkSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_pFormworkSpec;
}

/*----------------------------------------------------------------------------*/
/* get pulsed                                                                 */
/*----------------------------------------------------------------------------*/
bool Formwork::bPulsedLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_bPulsed;
}

/*----------------------------------------------------------------------------*/
/* set pulsed                                                                 */
/*----------------------------------------------------------------------------*/
void Formwork::vPulsed(bool bPulsed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  m_bPulsed = bPulsed;
}

/*----------------------------------------------------------------------------*/
/* get pulsed                                                                 */
/*----------------------------------------------------------------------------*/
bool Formwork::bPulsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  return m_bPulsed;
}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void Formwork::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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
void Formwork::vSpawnOpTasks(i8 iSessionId, i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    
    Stage* pStage = pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSpawnOpTasks(iSessionId, iNode);
    }
  }
  
}

#endif // WITH_FORMWORK

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
