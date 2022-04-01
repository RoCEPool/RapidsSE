/*============================================================================*/
/* Vertex.cpp -- Vertex of Pipeline                                           */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Deck.hpp"
#include "Vertex.hpp"
#include "VertexSpec.hpp"
#include "StageSpec.hpp"
#include "Pipeline.hpp"

#include "SP_PipeWork.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Vertex,CN_VERTEX)
__aNewFunction(Vertex,CN_VERTEX)
__vDeleteFunction(Vertex,CN_VERTEX)
__aFooterFunction(Vertex)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Vertex::Vertex(Pipeline*   pPipeline,
               Vertex*     pParentVertex,
               VertexSpec* pVertexSpec)
: ListNL_t<Vertex>(CN_VERTEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  m_pPipeline     = pPipeline;
  m_pParentVertex = pParentVertex;
  m_pVertexSpec   = pVertexSpec;

  m_pDeck = new Deck(this,pVertexSpec->pDeckSpec(),m_pVertexSpec);

  _checkInitialised_(CN_VERTEX);
  
  vCreatePipework();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vertex::~Vertex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  while (Vertex *pChildVertex = pPop())
    delete pChildVertex;
  
  delete m_pDeck;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* cancel the pipeline                                                        */
/*----------------------------------------------------------------------------*/
bool Vertex::bCancel(Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pDeck->pGet(0);
  
  // send cancelMarker to the start Op of the first stage of the pipeline
  pStage->vCancelPipeline(pTriggeringTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get formwork spec                                                          */
/*----------------------------------------------------------------------------*/
VertexSpec* Vertex::pPipeSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  return m_pVertexSpec;
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* Vertex::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  Stage*  pStage0 = m_pDeck->pGet(0);
  Op*     pOp0    = pStage0->pOp0();

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Vertex::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  i8 nOps = 0;
  
  i8 iOriginNodeNo = m_pPipeline->iOriginNodeNo();
  
  i8 nStages = this->nStages();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = m_pDeck->pGet(i_Stage);
    nOps += pStage->nOpsOnNode(iNode,iOriginNodeNo);
  }
  
  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Vertex::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Vertex::vSetInvoker(Task* pInvoker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nStages(); i_Stage++) {
    
    Stage* pStage = m_pDeck->pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSetInvoker(pInvoker);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void Vertex::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  Stage* pStage = m_pDeck->pGet(0);
  
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
void Vertex::vSpawnOpTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nStages(); i_Stage++) {
    
    Stage* pStage = m_pDeck->pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSpawnOpTasks();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get stage 0                                                                */
/*----------------------------------------------------------------------------*/
Stage* Vertex::pStage0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  Stage*  pStage0 = m_pDeck->pGet(0);

  return pStage0;
}

/*----------------------------------------------------------------------------*/
/* get number of stages                                                       */
/*----------------------------------------------------------------------------*/
i8 Vertex::nStages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  i8 nStages = m_pDeck->nItems();
  
  return nStages;
}

/*----------------------------------------------------------------------------*/
/* get stage set                                                              */
/*----------------------------------------------------------------------------*/
Deck* Vertex::pStaging(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
  return m_pDeck;
}

/*----------------------------------------------------------------------------*/
/* get stream 0                                                               */
/*----------------------------------------------------------------------------*/
Stream* Vertex::pStream0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);
  
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

void Vertex::vCreatePipework(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);

  i8 nStages = m_pVertexSpec->nStageSpecs();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pVertexSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(this,m_pDeck,pStageSpec);
    m_pDeck->vAppend(pStage);
    pStage->vCreateStreams();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
