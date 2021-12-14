/*============================================================================*/
/* JobReport.cpp -- JobReport                                                 */
/*============================================================================*/

#include "BASE.h"
#include "JobReport.hpp"
#include "JobList.hpp"
#include "Job.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Table.hpp"
#include "Pipeline.hpp"
#include "Stage.hpp"
#include "LogicUnit.hpp"
#include "Stream.hpp"
#include "Op.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,JobReport,CN_JOBREPORT)
__aNewFunction(JobReport,CN_JOBREPORT)
__vDeleteFunction(JobReport,CN_JOBREPORT)
__aFooterFunction(JobReport)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobReport::JobReport(i8 iJobNo, Job* pJob)
: Object(CN_JOBREPORT)
{
  _precondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);

  m_iJobNo   = iJobNo;
  m_tJobId   = C_UNKNOWN;
  m_pJob     = pJob;
  m_pJobTags = nullptr;

  m_i_pu_test   = C_UNKNOWN;
  m_i_pu_subset = C_UNKNOWN;
  m_i_pu_t      = C_UNKNOWN;
  m_i_pu_ft     = C_UNKNOWN;
  m_i_pu_j      = C_UNKNOWN;
  m_i_pu_fj     = C_UNKNOWN;
  m_i_pu_groups = C_UNKNOWN;

  m_iReponseTime = C_UNKNOWN;
  m_iMagnitude   = C_UNKNOWN;
  
  _checkInitialised_(CN_JOBREPORT);

  _postcondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobReport::~JobReport(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operators report                                                           */
/*----------------------------------------------------------------------------*/
eEC JobReport::eOperators(i8 iJobNo, Tub* pTub)
{
  _static_

  // find the participant job
  Job* pJob = pFindParticipantJob(iJobNo);

  // no such job ?
  if ( ! pJob )
    return EC_NO_SUCH_JOB;

  // create the jobReport
  JobReport* pJobReport = new JobReport(iJobNo, pJob);

  // find the internal jobOperators table
  Table* pJobOperatorsTable = AM::pTableFind(C_TDEF_JOBOPERATORS);
  StreamDef* pStreamDef = pJobOperatorsTable->pStreamDef();

  // get a working buffer from the tub
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
    // new a named buffer
    pNamedBuffer = new NamedBuffer(pStreamDef);

    // add the named buffer to the tub
    pTub->vAddBuffer(pNamedBuffer);
  }

  // get the pipeline
  Pipeline* pPipeline = *pJobReport->m_pJob->m_pJobLogicUnit->m_xpPipeline;

  // each stage in the pipeline
  for (i8 i_Stage = 0; i_Stage < pPipeline->nItems(); i_Stage++) {

    Stage* pStage = pPipeline->pGet(i_Stage);

    // each stream in the stage
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      // each op in each stream
      for (i8 i_Op = pStream->nItems() - 1; i_Op >= 0; i_Op--) {

        // get the stats for the op
        Op* pOp = pStream->pGet(i_Op);

        // skip operators that don't have a LU
        if ( ! pOp->pOpLogicUnit() )
          continue;

        Stats* pStats = *pOp->pOpLogicUnit()->m_xpStats;

        s32 sBytesAlloc;
        MagLib::vByteMag(pStats->m_nBytesAlloc, sBytesAlloc);

        s32 sBytesFree;
        MagLib::vByteMag(pStats->m_nFrees, sBytesFree);

        s32 sMaxBytes;
        MagLib::vByteMag(pStats->m_nMaxBytes, sMaxBytes);

        s32 sBytesRead;
        MagLib::vByteMag(pStats->m_nBytesRead, sBytesRead);

        s32 sBytesWrite;
        MagLib::vByteMag(pStats->m_nBytesWrite, sBytesWrite);

        // is their space in the buffer ?
        if (pNamedBuffer->nFreeBytes() < pNamedBuffer->nRowProperBytes() + 1) {

          // new a named buffer
          pNamedBuffer = new NamedBuffer(pStreamDef);

          // add athe named buffer to the tub
          pTub->vAddBuffer(pNamedBuffer);
        }

        // allot a row for the stats in the named buffer
        Row* pRow = pNamedBuffer->pAllotRow(C_VARGS_BEGIN
                                          ,iJobNo
                                          ,pOp->szStageName()
                                          ,pOp->szOpNm()
                                          ,pOp->tPtnId().iAsI8()
                                          ,pStats->iMicroseconds()
                                          ,pStats->m_nMaxQs
                                          ,pStats->m_nOperands
                                          ,pStats->m_nBuffIn
                                          ,pStats->m_nBuffOut
                                          ,pStats->m_nNewRowBuffer
                                          ,pStats->m_nDelRowBuffer
                                          ,pStats->m_nNewCsvBuffer
                                          ,pStats->m_nDelCsvBuffer
                                          ,pStats->m_nAllocs
                                          ,sBytesAlloc.azChar()
                                          ,pStats->m_nFrees
                                          ,sBytesFree.azChar()
                                          ,sMaxBytes.azChar()
                                          ,pStats->m_nReads
                                          ,sBytesRead.azChar()
                                          ,pStats->m_nWrites
                                          ,sBytesWrite.azChar()
                                          ,pStats->m_nParcelSends
                                          ,pStats->m_nObjectsSent
                                          ,pStats->m_nParcelRecvs
                                          ,pStats->m_nObjectsRecv
                                          ,C_VARGS_END);
      }
    }
  }

  // delete the jobReport
  delete pJobReport;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* report                                                                     */
/*----------------------------------------------------------------------------*/
eEC JobReport::eReport(i8 iJobNo, Tub* pTub)
{
  _static_

  // find the participant job
  Job* pJob = pFindOriginalJob(iJobNo);

  // no such job ?
  if ( ! pJob )
    return EC_NO_SUCH_JOB;

  // create the jobReport
  JobReport* pJobReport = new JobReport(iJobNo, pJob);

  // generate the report
  pJobReport->vGenerateReport(pTub);

  // delete the jobReport
  delete pJobReport;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* report                                                                     */
/*----------------------------------------------------------------------------*/
eEC JobReport::eReport(Job* pJob, Tub* pTub)
{
  _static_

  // create the jobReport
  JobReport* pJobReport = new JobReport(pJob->tJobId().uJobNo(), pJob);

  // generate the report
  pJobReport->vGenerateReport(pTub);

  // delete the jobReport
  delete pJobReport;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* find the job on the origin                                                 */
/*----------------------------------------------------------------------------*/
Job* JobReport::pFindOriginalJob(i8 iJobId)
{
  _static_

  //vTrace("bFindParts: %lld\n"_Stz,m_iJobNo);
  
  
  // form jobId from nodeNo.m_jobNo
  
  Job* pJob = nullptr;
  JobList* pRetiredJobs = AM::pRetiredJobs();
  
  pRetiredJobs->vGrabLock();
  
  for (SkipNode* pScan = pRetiredJobs->pFirst();
       pScan;
       pScan = pRetiredJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);
    
    i8 iJobNo = (i8)pJobScan->tJobId().uJobNo();
//    vTrace("Scan %p %lld (%s)\n"_Stz
//           ,pJobScan
//           ,iJobNo
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
    
    if (iJobNo == iJobId && C_UNKNOWN == pJobScan->iParticipantNode()) {
      pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

  if (pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }

  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find the participant job on the origin                                     */
/*----------------------------------------------------------------------------*/
Job* JobReport::pFindParticipantJob(i8 iJobId)
{
  _static_

  Job* pJob = nullptr;
  JobList* pRetiredJobs = AM::pRetiredJobs();

  pRetiredJobs->vGrabLock();

  for (SkipNode* pScan = pRetiredJobs->pFirst();
       pScan;
       pScan = pRetiredJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);

    i8 iJobNo = (i8)pJobScan->tJobId().uJobNo();
//    vTrace("Scan %p %lld (%s)\n"_Stz
//           ,pJobScan
//           ,iJobNo
//           ,C_UNKNOWN == pJobScan->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );

    if (iJobNo == iJobId && C_UNKNOWN != pJobScan->iParticipantNode()) {
      if (pJobScan->szJobNm()->bSame("pipeline"_Stz))
        pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

  if (pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }

  return pJob;
}

/*----------------------------------------------------------------------------*/
/*-- Private ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate the job report                                                    */
/*----------------------------------------------------------------------------*/
void JobReport::vGenerateReport(Tub* pTub) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);

  // get stats from job tags of job
  vStatsFromJobTags();

  // find the internal jobReport table
  Table* pJobReportTable = AM::pTableFind(C_TDEF_JOBREPORT);
  StreamDef* pStreamDef = pJobReportTable->pStreamDef();

  // get a working buffer from the tub
  NamedBuffer* pNamedBuffer = _pNamedBuffer(pTub->pWorkingBuffer());

  if (! pNamedBuffer) {
  // new a named buffer
  pNamedBuffer = new NamedBuffer(pStreamDef);

  // add the named buffer to the tub
  pTub->vAddBuffer(pNamedBuffer);
  }

  // is their space in the buffer ?
  if (pNamedBuffer->nFreeBytes() < pNamedBuffer->nRowProperBytes() + 1) {

  // new a named buffer
  pNamedBuffer = new NamedBuffer(pStreamDef);

  // add athe named buffer to the tub
  pTub->vAddBuffer(pNamedBuffer);
  }

  // allot a row for the stats in the named buffer
  Row* pRow = pNamedBuffer->pAllotRow(C_VARGS_BEGIN,
                                      m_i_pu_test,
                                      m_i_pu_subset,
                                      m_i_pu_t,
                                      m_i_pu_ft,
                                      m_i_pu_j,
                                      m_i_pu_fj,
                                      m_i_pu_groups,
                                      m_iReponseTime,
                                      m_iMagnitude,
                                      C_VARGS_END);
}

/*----------------------------------------------------------------------------*/
/* get stats from jobTags                                                     */
/*----------------------------------------------------------------------------*/
void JobReport::vStatsFromJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);

  // get jobTags
  m_pJobTags = &m_pJob->m_oJobTags;

  //m_pJobTags->vContentDump();

  m_i_pu_test   = StrLib::iToI8(m_pJobTags->szValue("<pu_test>"_Stz)->azChar());
  m_i_pu_subset = StrLib::iToI8(m_pJobTags->szValue("<pu_subset>"_Stz)->azChar());
  m_i_pu_t      = StrLib::iToI8(m_pJobTags->szValue("<pu_t>"_Stz)->azChar());
  m_i_pu_ft     = StrLib::iToI8(m_pJobTags->szValue("<pu_ft>"_Stz)->azChar());
  m_i_pu_j      = StrLib::iToI8(m_pJobTags->szValue("<pu_j>"_Stz)->azChar());
  m_i_pu_fj     = StrLib::iToI8(m_pJobTags->szValue("<pu_fj>"_Stz)->azChar());
  m_i_pu_groups = StrLib::iToI8(m_pJobTags->szValue("<pu_groups>"_Stz)->azChar());

//  vTrace("puTags: \n"
//           "pu_test = %lld; "
//           "pu_subset=%lld; "
//           "pu_t=%lld; "
//           "pu_ft=%lld; "
//           "pu_j=%lld; "
//           "pu_fj=%lld; "
//           "pu_groups=%lld\n"_Stz
//    ,m_i_pu_test
//    ,m_i_pu_subset
//    ,m_i_pu_t
//    ,m_i_pu_ft
//    ,m_i_pu_j
//    ,m_i_pu_fj
//    ,m_i_pu_groups);

  // get response

  m_iReponseTime = m_pJob->m_oTimer.iMicroseconds();
  m_iMagnitude   = m_pJob->m_oTimer.iMagnitude();

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
