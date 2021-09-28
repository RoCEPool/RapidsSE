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
JobReport::JobReport(i8 iJobNo)
: Object(CN_JOBREPORT)
{
  _precondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);

  m_iJobNo   = iJobNo;
  m_tJobId   = C_UNKNOWN;
  m_pJob     = nullptr;
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
void JobReport::vOperators(i8 iJobNo, Tub* pTub)
{
  _static_

  //vTrace("JobReport: %lld\n"_Stz,iJobNo);

  JobReport* pJobReport = new JobReport(iJobNo);

  if ( ! pJobReport->bFindParticipantJob() )
    return;

  Table* pJobOperatorsTable = AM::pTableFind(C_TDEF_JOBOPERATORS);
  StreamDef* pStreamDef = pJobOperatorsTable->pStreamDef();

  // new a named buffer
  NamedBuffer* pNamedBuffer = new NamedBuffer(pStreamDef);

  // add athe named buffer to the tub
  pTub->vAddBuffer(pNamedBuffer);

  Pipeline* pPipeline = *pJobReport->m_pJob->m_pJobLogicUnit->m_xpPipeline;

  for (i8 i_Stage = 0; i_Stage < pPipeline->nItems(); i_Stage++) {

    Stage* pStage = pPipeline->pGet(i_Stage);

    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      for (i8 i_Op = pStream->nItems() - 1; i_Op >= 0; i_Op--) {

        Op* pOp = pStream->pGet(i_Op);
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


        if (pNamedBuffer->nFreeBytes() < pNamedBuffer->nRowProperBytes() + 1) {

          // new a named buffer
          pNamedBuffer = new NamedBuffer(pStreamDef);

          // add athe named buffer to the tub
          pTub->vAddBuffer(pNamedBuffer);
        }

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
        if ( ! pRow ) {

        }

      }
    }
  }

  delete pJobReport;
}

/*----------------------------------------------------------------------------*/
/* report                                                                     */
/*----------------------------------------------------------------------------*/
NamedBuffer* JobReport::pReport(i8 iJobNo)
{
  _static_

  //vTrace("JobReport: %lld\n"_Stz,iJobNo);
  
  JobReport* pJobReport = new JobReport(iJobNo);
  
  pJobReport->vReportJob();

  Table* pJobReportTable = AM::pTableFind(C_TDEF_JOBREPORT);
  StreamDef* pStreamDef = pJobReportTable->pStreamDef();
  NamedBuffer* pNamedBuffer = new NamedBuffer(pStreamDef);

  Row* pRow = pNamedBuffer->pAllotRow(C_VARGS_BEGIN,
                                      pJobReport->m_i_pu_test,
                                      pJobReport->m_i_pu_subset,
                                      pJobReport->m_i_pu_t,
                                      pJobReport->m_i_pu_ft,
                                      pJobReport->m_i_pu_j,
                                      pJobReport->m_i_pu_fj,
                                      pJobReport->m_i_pu_groups,
                                      pJobReport->m_iReponseTime,
                                      pJobReport->m_iMagnitude,
                                      C_VARGS_END);

  delete pJobReport;

  return pNamedBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft the report for job                                                   */
/*----------------------------------------------------------------------------*/
void JobReport::vCraftReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);
  
  //vTrace("vCraftReport: %lld\n"_Stz,m_iJobNo);
  
  // insert into namedBuffer??

}

/*----------------------------------------------------------------------------*/
/* find the job on the origin                                                 */
/*----------------------------------------------------------------------------*/
bool JobReport::bFindOriginJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);

  //vTrace("bFindParts: %lld\n"_Stz,m_iJobNo);
  
  
  // form jobId from nodeNo.m_jobNo
  
  m_pJob = nullptr;
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
    
    if (iJobNo == m_iJobNo && C_UNKNOWN == pJobScan->iParticipantNode()) {
      m_pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

  if (m_pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }
  else {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* find the participant job on the origin                                     */
/*----------------------------------------------------------------------------*/
bool JobReport::bFindParticipantJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);

  //vTrace("bFindParts: %lld\n"_Stz,m_iJobNo);


  // form jobId from nodeNo.m_jobNo

  m_pJob = nullptr;
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

    if (iJobNo == m_iJobNo && C_UNKNOWN != pJobScan->iParticipantNode()) {
      if (pJobScan->szJobNm()->bSame("pipeline"_Stz))
        m_pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

  if (m_pJob) {
//    vTrace("Found %p %lld (%s)\n"_Stz
//           ,m_pJob
//           ,(i8)m_pJob->tJobId().uJobNo()
//           ,C_UNKNOWN == m_pJob->iParticipantNode()
//             ? "origin"_Stz
//             : "participant"_Stz
//           );
  }
  else {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* find all the parts                                                         */
/*----------------------------------------------------------------------------*/
bool JobReport::bFindParts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);

  //vTrace("bFindParts: %lld\n"_Stz,m_iJobNo);


  // form jobId from nodeNo.m_jobNo

  if ( ! bFindOriginJob() )
    return false;

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

  return true;
}

/*----------------------------------------------------------------------------*/
/* report on job                                                              */
/*----------------------------------------------------------------------------*/
void JobReport::vReportJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);
  
  //vTrace("vReportJob: %lld\n"_Stz,m_iJobNo);

  if (bFindParts())
    vCraftReport();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
