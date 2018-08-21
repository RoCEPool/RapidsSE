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

/*----------------------------------------------------------------------------*/
/* vtrace the job class for the jobcat prompt function                        */
/*----------------------------------------------------------------------------*/
void JobClass::vJobCat(void)
{
  vTrace("%20s %10s %20s : %10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         m_aName,
         aJK[m_eKind],
         m_aComment,
         
         m_bResources ? "_chk/rel"_Stz : "-"_Stz,
         
         m_aSeedRun ? m_aSeedRun : "-"_Stz,
         m_aSeedReq ? m_aSeedReq : "-"_Stz,
         m_aSeedRcv ? m_aSeedRcv : "-"_Stz,
         m_aSeedExe ? m_aSeedExe : "-"_Stz,
         m_aSeedEnd ? m_aSeedEnd : "-"_Stz,
         m_aSeedCan ? m_aSeedCan : "-"_Stz
         );

}

/*----------------------------------------------------------------------------*/
/* vtrace the header for the jobcat prompt function                           */
/*----------------------------------------------------------------------------*/
void JobClass::vJobCatHeader(void)
{
  vTrace("%20s %10s %20s : %10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         "Job Name"_Stz,
         "Kind"_Stz,
         "Comment"_Stz,
         
         "Resources"_Stz,
         "Run Seed"_Stz,
         "Req Seed"_Stz,
         "Rcv Seed"_Stz,
         "Exe Seed"_Stz,
         "End Seed"_Stz,
         "Can Seed"_Stz
         );
  
  s32 sBar10 = "----------"_Stz;
  s32 sBar20 = "--------------------"_Stz;
  
  vTrace("%20s %10s %20s : "
         "%10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         sBar20.aStr(),sBar10.aStr(),sBar20.aStr(),sBar10.aStr(),
         
         sBar20.aStr(),sBar20.aStr(),sBar20.aStr(),sBar20.aStr(),sBar20.aStr(),sBar20.aStr()
         );
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
