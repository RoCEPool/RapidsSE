/*============================================================================*/
/* JobReport.cpp -- JobReport                                                 */
/*============================================================================*/

#include "BASE.h"
#include "JobReport.hpp"
#include "JobList.hpp"
#include "Job.hpp"

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

  m_i_pu_puSet  = C_UNKNOWN;
  m_i_pu_t      = C_UNKNOWN;
  m_i_pu_ft     = C_UNKNOWN;
  m_i_pu_j      = C_UNKNOWN;
  m_i_pu_fj     = C_UNKNOWN;
  m_i_pu_groups = C_UNKNOWN;

  m_iReponseTime = C_UNKNOWN;
  
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
/* report                                                                     */
/*----------------------------------------------------------------------------*/
void JobReport::vReport(i8 iJobNo)
{
  _static_

  vTrace("JobReport: %lld\n"_Stz,iJobNo);
  
  JobReport* pJobReport = new JobReport(iJobNo);
  
  pJobReport->vReportJob();
  
  delete pJobReport;
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
  
  vTrace("vCraftReport: %lld\n"_Stz,m_iJobNo);
  
  // insert into namedBuffer??

}

/*----------------------------------------------------------------------------*/
/* find all the parts                                                         */
/*----------------------------------------------------------------------------*/
bool JobReport::bFindParts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);

  vTrace("bFindParts: %lld\n"_Stz,m_iJobNo);
  
  
  // form jobId from nodeNo.m_jobNo
  
  m_pJob = nullptr;
  JobList* pRetiredJobs = AM::pRetiredJobs();
  
  pRetiredJobs->vGrabLock();
  
  for (SkipNode* pScan = pRetiredJobs->pFirst();
       pScan;
       pScan = pRetiredJobs->pNext(pScan)) {
    Job* pJobScan = _pJob(pScan->m_pObject);
    
    i8 iJobNo = (i8)pJobScan->tJobId().uJobNo();
    vTrace("Scan %p %lld (%s)\n"_Stz
           ,pJobScan
           ,iJobNo
           ,C_UNKNOWN == pJobScan->iParticipantNode()
             ? "origin"_Stz
             : "participant"_Stz
           );
    
    if (iJobNo == m_iJobNo && C_UNKNOWN == pJobScan->iParticipantNode()) {
      m_pJob = pJobScan;
    }
  }

  pRetiredJobs->vDropLock();

  if (m_pJob) {
    vTrace("Found %p %lld (%s)\n"_Stz
           ,m_pJob
           ,(i8)m_pJob->tJobId().uJobNo()
           ,C_UNKNOWN == m_pJob->iParticipantNode()
             ? "origin"_Stz
             : "participant"_Stz
           );
  }
  
  // get jobTags
  m_pJobTags = &m_pJob->m_oJobTags;

  //m_pJobTags->vContentDump();

  m_i_pu_puSet  = StrLib::iToI8(m_pJobTags->szValue("<pu_puSet>"_Stz)->azChar());
  m_i_pu_t      = StrLib::iToI8(m_pJobTags->szValue("<pu_t>"_Stz)->azChar());
  m_i_pu_ft     = StrLib::iToI8(m_pJobTags->szValue("<pu_ft>"_Stz)->azChar());
  m_i_pu_j      = StrLib::iToI8(m_pJobTags->szValue("<pu_j>"_Stz)->azChar());
  m_i_pu_fj     = StrLib::iToI8(m_pJobTags->szValue("<pu_fj>"_Stz)->azChar());
  m_i_pu_groups = StrLib::iToI8(m_pJobTags->szValue("<pu_groups>"_Stz)->azChar());

  vTrace("puTags: \n"
    "pu_puSet=%lld;  pu_t=%lld; pu_ft=%lld; pu_j=%lld; pu_fj=%lld; pu_groups=%lld\n"_Stz
    ,m_i_pu_puSet
    ,m_i_pu_t
    ,m_i_pu_ft
    ,m_i_pu_j
    ,m_i_pu_fj
    ,m_i_pu_groups);

  // get response

  return true;
}

/*----------------------------------------------------------------------------*/
/* report on job                                                              */
/*----------------------------------------------------------------------------*/
void JobReport::vReportJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);
  
  vTrace("vReportJob: %lld\n"_Stz,m_iJobNo);

  if (bFindParts())
    vCraftReport();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
