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
  
  m_nMilliseconds = C_UNKNOWN;
  m_nThreadsIdle = 0;
  
  m_pNext  = nullptr;
  
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
    
  vCollect(pJob);
  
  //vPrint();
  
  pJob->vDropLock();
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* get first pipeline metrics                                                 */
/*----------------------------------------------------------------------------*/
PipelineMetrics* JobMetrics::pGetPipelineMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  d_MetricsCursor = 0;
  
  PipelineMetrics* pPipelineMetrics =
    _pPipelineMetrics(m_oMetricsWsp.aAddr(d_MetricsCursor));
  
  d_MetricsCursor += sizeof(PipelineMetrics);
  
  return pPipelineMetrics;
}

/*----------------------------------------------------------------------------*/
/* get first stage metrics                                                    */
/*----------------------------------------------------------------------------*/
StageMetrics* JobMetrics::pGetStageMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  StageMetrics* pStageMetrics =
    _pStageMetrics(m_oMetricsWsp.aAddr(d_MetricsCursor));
  
  d_MetricsCursor += sizeof(StageMetrics);
  
  return pStageMetrics;
}

/*----------------------------------------------------------------------------*/
/* get first stream metrics                                                   */
/*----------------------------------------------------------------------------*/
StreamMetrics* JobMetrics::pGetStreamMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  StreamMetrics* pStreamMetrics =
    _pStreamMetrics(m_oMetricsWsp.aAddr(d_MetricsCursor));
  
  d_MetricsCursor += sizeof(StreamMetrics);
  
  return pStreamMetrics;
}

/*----------------------------------------------------------------------------*/
/* get first op metrics                                                       */
/*----------------------------------------------------------------------------*/
OpMetrics* JobMetrics::pGetOpMetrics(i8& d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  OpMetrics* pOpMetrics =
    _pOpMetrics(m_oMetricsWsp.aAddr(d_MetricsCursor));
  
  d_MetricsCursor += sizeof(OpMetrics);
  
  return pOpMetrics;
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
/* get next job metrics                                                       */
/*----------------------------------------------------------------------------*/
JobMetrics* JobMetrics::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* link to next job metrics                                                   */
/*----------------------------------------------------------------------------*/
void JobMetrics::vNext(JobMetrics* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* get milliseconds into job                                                  */
/*----------------------------------------------------------------------------*/
i8 JobMetrics::nMilliseconds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return m_nMilliseconds;
}

/*----------------------------------------------------------------------------*/
/* get threads idle                                                           */
/*----------------------------------------------------------------------------*/
i8 JobMetrics::nThreadsIdle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  return m_nThreadsIdle;
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next op metrics                                                        */
/*----------------------------------------------------------------------------*/
void JobMetrics::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  vTrace("JobMetrics (%%free %lld)\n"_Stz,m_oMetricsWsp.iPctFree());
  
  i8 d_MetricsCursor = C_UNKNOWN;
  
  PipelineMetrics* pPipelineMetrics = pGetPipelineMetrics(d_MetricsCursor);
  
  vTrace("  Pipeline %s\n"_Stz
         ,pPipelineMetrics->szPipelineNm());

  while (CN_STAGEMETRICS == eNextClassNo(d_MetricsCursor)) {
    StageMetrics* pStageMetrics = pGetStageMetrics(d_MetricsCursor);
    vTrace("    Stage %lld %s\n"_Stz
           ,pStageMetrics->i_Stage()
           ,pStageMetrics->szStageNm());

    while (CN_STREAMMETRICS == eNextClassNo(d_MetricsCursor)) {
      StreamMetrics* pStreamMetrics = pGetStreamMetrics(d_MetricsCursor);
      vTrace("      Stream %lld\n"_Stz
             ,pStreamMetrics->i_Stream());

      while (CN_OPMETRICS == eNextClassNo(d_MetricsCursor)) {
        OpMetrics* pOpMetrics = pGetOpMetrics(d_MetricsCursor);
        vTrace("        Op %lld %s queued=%lld\n"_Stz
               ,pOpMetrics->i_Op()
               ,pOpMetrics->szOpNm()
               ,pOpMetrics->nQueued()
               );

      }
      
    }

  }

  
}

/*----------------------------------------------------------------------------*/
/* use this job metrics to update the pipeline on this node                   */
/*----------------------------------------------------------------------------*/
bool JobMetrics::bUpdate(bool bAppendList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);

  //vTrace("JobMetrics jobId=%lld\n"_Stz,m_tJobId.iAsI8());

  //vPrint();
  
  JobList* pParticipatingJobs = AM::pParticipatingJobs();
  
  Job* pJob = pParticipatingJobs->pFindJob(m_tJobId,true);
  
  if (! pJob) {
    vTrace("JobMetrics job not found jobId=%lld\n"_Stz,m_tJobId.iAsI8());
    return false;
  }
  
  
  Pipeline* pPipeline = pJob->pPipeline_job();
  if (! pPipeline) {
    pJob->vDropLock();
    vTrace("JobMetrics pipeline not found jobId=%lld\n"_Stz,m_tJobId.iAsI8());
    return false;
  }
  
  
  
  
  i8 d_MetricsCursor = C_UNKNOWN;
  
  PipelineMetrics* pPipelineMetrics = pGetPipelineMetrics(d_MetricsCursor);
  if (! pPipelineMetrics) {
    pJob->vDropLock();
    vTrace("JobMetrics pPipelineMetrics not found jobId=%lld\n"_Stz,m_tJobId.iAsI8());
    return false;
  }
  
//  vTrace("  Pipeline %s\n"_Stz
//         ,pPipelineMetrics->szPipelineNm());

  while (CN_STAGEMETRICS == eNextClassNo(d_MetricsCursor)) {
    StageMetrics* pStageMetrics = pGetStageMetrics(d_MetricsCursor);
//    vTrace("    Stage %lld %s\n"_Stz
//           ,pStageMetrics->i_Stage()
//           ,pStageMetrics->szStageNm());
    
    i8 i_Stage = pStageMetrics->i_Stage();
    Stage* pStage = pPipeline->pGet(i_Stage);

    while (CN_STREAMMETRICS == eNextClassNo(d_MetricsCursor)) {
      StreamMetrics* pStreamMetrics = pGetStreamMetrics(d_MetricsCursor);
//      vTrace("      Stream %lld\n"_Stz
//             ,pStreamMetrics->i_Stream());

      i8 i_Stream = pStreamMetrics->i_Stream();
      Stream* pStream = pStage->pGet(i_Stream);
      
      while (CN_OPMETRICS == eNextClassNo(d_MetricsCursor)) {
        OpMetrics* pOpMetrics = pGetOpMetrics(d_MetricsCursor);
//        vTrace("        Op %lld %s queued=%lld\n"_Stz
//               ,pOpMetrics->i_Op()
//               ,pOpMetrics->szOpNm()
//               ,pOpMetrics->nQueued()
//               );

        i8 i_Op = pOpMetrics->i_Op();
        Op* pOp = pStream->pGet(i_Op);
        
        i8 nQueued = pOpMetrics->nQueued();
        pOp->vMetricsQueued(nQueued);
      }
      
    }

  }


  if (bAppendList) {
    
    //JobList* pOriginatingJobs = AM::pOriginatingJobs();
    
    //Job* pJob_originator = pOriginatingJobs->pFindJob(m_tJobId,true);
    
    //if (pJob_originator) {
      //vTrace("JobMetrics update job =%p\n"_Stz,pJob);
      pJob->vJobMetricsAdd(this);
      //pJob->vDropLock();
    //}
    //else {
    //  vTrace("JobMetrics originatiing job not found jobId=%lld\n"_Stz,m_tJobId.iAsI8());
    //}
  }
  
  pJob->vDropLock();

  
  return true;
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
  
  m_nThreadsIdle = AM::SCHEDULER::nIdleThreads();

  Timer* pProgressTimer = pJob->pProgressTimer();
  pProgressTimer->vUpdate();
  m_nMilliseconds = pProgressTimer->iMilliseconds();
  
  Pipeline* pPipeline = pJob->pPipeline_job();
  
  new(& m_oMetricsWsp) PipelineMetrics(pPipeline->szPipelineNm());
  
  i8 nStages = pPipeline->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pPipeline->pGet(i_Stage);
    
    new(& m_oMetricsWsp) StageMetrics(pStage->i_Stage()
                                      ,pStage->szStageNm());

    i8 nStreams = pStage->nItems();
    for (i8 i_Stream = 0; i_Stream < nStreams; i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);
      
      new(& m_oMetricsWsp) StreamMetrics(pStream->i_Stream());

      i8 nOps = pStream->nItems();
      for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
        Op* pOp = pStream->pGet(i_Op);
        
        new(& m_oMetricsWsp) OpMetrics(pOp->i_Op()
                                       ,pOp->szOpNm()
                                       ,pOp->nQueuedOperands());
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* collect job metrics                                                        */
/*----------------------------------------------------------------------------*/
eCN JobMetrics::eNextClassNo(i8 d_MetricsCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBMETRICS,_FOOTER_);
  
  Object* pObj = pObject(d_MetricsCursor);
  
  if (! pObj)
    return CN_NONE;
  
  eCN eClassNo = pObj->eGetClassNo();
  
  return eClassNo;
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
