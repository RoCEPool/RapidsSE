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
  
  Stage*  pStage0 = pGet(0);
  Op*     pOp0    = pStage0->pOp0();

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
void Formwork::vSpawnOpTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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
Stage* Formwork::pStage0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
  Stage*  pStage0 = pGet(0);

  return pStage0;
}

/*----------------------------------------------------------------------------*/
/* get stream 0                                                               */
/*----------------------------------------------------------------------------*/
Stream* Formwork::pStream0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMWORK,_FOOTER_);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
