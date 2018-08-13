/*============================================================================*/
/* JobClass.cpp -- a description of a job class                               */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::_pJobClass(i8 iAddr)
{
  JobClass* pJobClass = reinterpret_cast<JobClass*>(iAddr);
  
  return pJobClass;
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the new-function                               */
/*----------------------------------------------------------------------------*/
Job* JobClass::pNewJob(Job* pArg)
{
  Job* pNewJob = (*m_fNew)(pArg);
  
  pNewJob->m_pJobClass = this;
  pNewJob->m_eJobTrace = m_eTrace;

  return pNewJob;
}

/*----------------------------------------------------------------------------*/
/* delete the job by running the delete-function                              */
/*----------------------------------------------------------------------------*/
void JobClass::vDeleteJob(Interface::SEitf* pSEitf, Job* pJob)
{
  (*m_fDelete)(pSEitf, pJob);
}

/*----------------------------------------------------------------------------*/
/* end the job by running the post-function                                   */
/*----------------------------------------------------------------------------*/
void JobClass::vPostJob(Interface::SEitf* pSEitf, Job* pJob)
{
  (*m_fPost)(pSEitf, pJob);
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the pre-function                                */
/*----------------------------------------------------------------------------*/
Job* JobClass::pPreJob(Interface::SEitf* pSEitf)
{
  Job* pJob = (*m_fPre)(pSEitf);

  pJob->m_pJobClass = this;
  pJob->m_eJobTrace = m_eTrace;
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* pre-send function                                                          */
/*----------------------------------------------------------------------------*/
void JobClass::vSendJob(Job* pJob)
{
  if (m_fSend)
    (*m_fSend)(pJob);
}

/*----------------------------------------------------------------------------*/
/* post-receive fucntion                                                      */
/*----------------------------------------------------------------------------*/
void JobClass::vRecvJob(Job* pJob)
{
  if (m_fRecv)
    (*m_fRecv)(pJob);
}

/*----------------------------------------------------------------------------*/
/* show the job                                                               */
/*----------------------------------------------------------------------------*/
void JobClass::vShowJob(Job* pJob)
{
  (*m_fShow)(pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
