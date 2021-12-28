/*============================================================================*/
/* JobMetrics.cpp -- Job Metrics                                              */
/*============================================================================*/

#include "BASE.h"
#include "JobMetrics.hpp"

#include "Job.hpp"
#include "JobList.hpp"

#include "Pipeline.hpp"
#include "Stage.hpp"
#include "Stream.hpp"
#include "Op.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,JobMetrics,CN_JOBMETRICS)
__aNewFunction(JobMetrics,CN_JOBMETRICS)
__vDeleteFunction(JobMetrics,CN_JOBMETRICS)
__aFooterFunction(JobMetrics)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobMetrics::JobMetrics(JobId tJobId)
: Operand(CN_JOBMETRICS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  m_tJobId = tJobId;
  
  _checkInitialised_(CN_JOBMETRICS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobMetrics::~JobMetrics(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get set of jobs                                                            */
/*----------------------------------------------------------------------------*/
void JobMetrics::vJobSet(IntStack* pIntStack)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  JobList* pParticipatingJobs = AM::pParticipatingJobs();
  
  pParticipatingJobs->vGrabLock();
  
  SkipNode* pJobNode;
  
  for (pJobNode = pParticipatingJobs->pFirst();
       pJobNode;
       pJobNode = pParticipatingJobs->pNext(pJobNode)) {
    
    Job* pJob = _pJob(pJobNode->m_pObject);
    
    JobId tJobId = pJob->tJobId();
    
    i8 iJob = tJobId.iAsI8();
    
    pIntStack->vPush(iJob);
  }

  pParticipatingJobs->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* repair the int stack                                                      */
/*----------------------------------------------------------------------------*/
void JobMetrics::vRepair(Object* pObject)
{
  _static_

  IntStack* pIntStack = _pIntStack(pObject);

  delete pIntStack;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect job metrics                                                        */
/*----------------------------------------------------------------------------*/
bool JobMetrics::bCollect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  JobList* pParticipatingJobs = AM::pParticipatingJobs();
  
  Job* pJob = pParticipatingJobs->pFindJob(m_tJobId,true);
  
  if (! pJob)
    return false;
  
  //vTrace("Found Job %lld %p\n"_Stz,m_tJobId.iAsI8(),pJob);
    
  //vCollect(pJob);
  
  //vPrint();
  
  pJob->vDropLock();
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* get first pipeline metrics                                                 */
/*----------------------------------------------------------------------------*/
PipelineMetrics* JobMetrics::pFirstPipelineMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first stage metrics                                                    */
/*----------------------------------------------------------------------------*/
StageMetrics* JobMetrics::pFirstStageMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first stream metrics                                                   */
/*----------------------------------------------------------------------------*/
StreamMetrics* JobMetrics::pFirstStreamMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first op metrics                                                       */
/*----------------------------------------------------------------------------*/
OpMetrics* JobMetrics::pFirstOpMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get metrics wsp                                                            */
/*----------------------------------------------------------------------------*/
MetricsWsp* JobMetrics::pMetricsWsp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return & m_oMetricsWsp;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next stage metrics                                                     */
/*----------------------------------------------------------------------------*/
StageMetrics* JobMetrics::pNextStageMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next stream metrics                                                    */
/*----------------------------------------------------------------------------*/
StreamMetrics* JobMetrics::pNextStreamMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next op metrics                                                        */
/*----------------------------------------------------------------------------*/
OpMetrics* JobMetrics::pNextOpMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next op metrics                                                        */
/*----------------------------------------------------------------------------*/
void JobMetrics::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  vTrace("JobMetrics\n"_Stz);
  
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect job metrics                                                        */
/*----------------------------------------------------------------------------*/
void JobMetrics::vCollect(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);
  
  Pipeline* pPipeline = pJob->pPipeline_job();
  new(& m_oMetricsWsp) PipelineMetrics(pPipeline->szPipelineNm());
  
  i8 nStages = pPipeline->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pPipeline->pGet(i_Stage);
    new(& m_oMetricsWsp) StageMetrics(i_Stage,pStage->szStageNm());

    i8 nStreams = pStage->nItems();
    for (i8 i_Stream = 0; i_Stream < nStreams; i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);
      new(& m_oMetricsWsp) StreamMetrics(i_Stream);

      i8 nOps = pStream->nItems();
      for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
        Op* pOp = pStream->pGet(i_Op);
        new(& m_oMetricsWsp) OpMetrics(i_Op,pOp->szOpNm(),
                                       pOp->nQueuedOperands());
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* collect job metrics                                                        */
/*----------------------------------------------------------------------------*/
Object* JobMetrics::pObject(i8 d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);
  
  Object* pObject = _pObject(m_oMetricsWsp.aAddr(d_MetricsCursor));
  
  return pObject;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
