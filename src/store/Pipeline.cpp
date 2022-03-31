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
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "VertexSpec.hpp"
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
Pipeline::Pipeline(Job*        pPipelineJob,
                   VertexSpec* pVertexSpecRoot,
                   i8          iOriginNodeNo)
: Object(CN_PIPELINE)
, m_okDebugLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_sPipelineNm   = pVertexSpecRoot->szVertexNm();
  
  m_pVertexRoot     = nullptr;
  
  m_pPipelineJob  = pPipelineJob;
  m_pVertexSpecRoot     = pVertexSpecRoot;
  m_iOriginNodeNo = iOriginNodeNo;

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
  
  m_pVertexRoot = new Vertex(this,nullptr,m_pVertexSpecRoot);
  //vCreatePipework();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pipeline::~Pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  delete m_pVertexRoot;
  
  delete m_pVertexSpecRoot;
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
  
  bool bCancelled = m_pVertexRoot->bCancel(pTriggeringTask);
  
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
/* dump this pipeline (and optionally all subpipes recursively)               */
/*----------------------------------------------------------------------------*/
void Pipeline::vDump(bool bRecursive)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
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
i8 Pipeline::iOriginNodeNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_iOriginNodeNo;
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
  
  Op* pOp0 = m_pVertexRoot->pOp0();

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Pipeline::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  i8 nOps = m_pVertexRoot->nOpsOnNode(iNode);
    
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
VertexSpec* Pipeline::pPipeSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pVertexSpecRoot;
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
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Pipeline::vSetInvoker(Task* pInvoker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  m_pVertexRoot->vSetInvoker(pInvoker);

}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void Pipeline::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_pVertexRoot->vSetupJob();
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Pipeline::vSpawnOpTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  m_pVertexRoot->vSpawnOpTasks();
}

/*----------------------------------------------------------------------------*/
/* get stage 0                                                                */
/*----------------------------------------------------------------------------*/
Stage* Pipeline::pStage0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  Stage* pStage0 = m_pVertexRoot->pStage0();

  return pStage0;
}

/*----------------------------------------------------------------------------*/
/* get stream 0                                                               */
/*----------------------------------------------------------------------------*/
Stream* Pipeline::pStream0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  Stream* pStream0 = m_pVertexRoot->pStream0();

  return pStream0;
}

/*----------------------------------------------------------------------------*/
/* get root of vertex tree                                                    */
/*----------------------------------------------------------------------------*/
Vertex* Pipeline::pVertexRoot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pVertexRoot;
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

  m_pVertexRoot->vCreatePipework();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
