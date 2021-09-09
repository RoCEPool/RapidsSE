/*============================================================================*/
/* Jobs.cpp -- Jobs                                                           */
/*============================================================================*/

#include "BASE.h"

#include "Jobs.hpp"
#include "Job.hpp"

#include "JobList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Jobs,CN_JOBS)
__aNewFunction(Jobs,CN_JOBS)
__vDeleteFunction(Jobs,CN_JOBS)
__aFooterFunction(Jobs)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Jobs::Jobs(void)
: Object(CN_JOBS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
  
  m_pDemonJobs         = nullptr;
  m_pLocalJobs         = nullptr;
  m_pOriginatingJobs   = nullptr;
  m_pParticipatingJobs = nullptr;
  m_pRetiredJobs       = nullptr;

  _checkInitialised_(CN_JOBS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Jobs::~Jobs(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create job lists                                                           */
/*----------------------------------------------------------------------------*/
void Jobs::vCreateJobLists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  m_pDemonJobs         = new JobList(STORE::bJobCompare_id, JL_DEMON);
  m_pLocalJobs         = new JobList(STORE::bJobCompare_id, JL_LOCAL);
  m_pOriginatingJobs   = new JobList(STORE::bJobCompare_id, JL_ORIGINATING);
  m_pParticipatingJobs = new JobList(STORE::bJobCompare_id, JL_PARTICIPATING);
  
  m_pRetiredJobs       = new JobList(nullptr, JL_RETIRED);
}

/*----------------------------------------------------------------------------*/
/* get demon job list                                                         */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pDemonJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pDemonJobs;
}

/*----------------------------------------------------------------------------*/
/* get local job list                                                         */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pLocalJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pLocalJobs;
}

/*----------------------------------------------------------------------------*/
/* get originating job list                                                   */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pOriginatingJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pOriginatingJobs;
}

/*----------------------------------------------------------------------------*/
/* get participating job list                                                 */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pParticipatingJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pParticipatingJobs;
}

/*----------------------------------------------------------------------------*/
/* get retired job list                                                       */
/*----------------------------------------------------------------------------*/
JobList* Jobs::pRetiredJobs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBS,_FOOTER_);

  return m_pRetiredJobs;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
