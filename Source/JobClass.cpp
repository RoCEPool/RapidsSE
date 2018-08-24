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
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::_pJobClass(void* aAddr)
{
  JobClass* pJobClass = reinterpret_cast<JobClass*>(aAddr);
  
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
  s32 sFunctions(""_Stz);
  
  sFunctions.vConcat(m_fPre ?    "p"_Stz : " "_Stz);
  sFunctions.vConcat(m_fPost ?   "P"_Stz : " "_Stz);
  sFunctions.vConcat(m_fNew ?    "n"_Stz : " "_Stz);
  sFunctions.vConcat(m_fDelete ? "d"_Stz : " "_Stz);
  sFunctions.vConcat(m_fSend ?   "s"_Stz : " "_Stz);
  sFunctions.vConcat(m_fRecv ?   "r"_Stz : " "_Stz);
  sFunctions.vConcat(m_fShow ?   "."_Stz : " "_Stz);

  s32 sSeedRun(m_aSeedRun ? m_aSeedRun : "-"_Stz);
  sSeedRun.vConcat(m_bSeedsSpawned[SS_SEED_RUN] ? "+"_Stz : " "_Stz);

  s32 sSeedReq(m_aSeedReq ? m_aSeedReq : "-"_Stz);
  sSeedReq.vConcat(m_bSeedsSpawned[SS_SEED_REQ] ? "+"_Stz : " "_Stz);

  s32 sSeedRcv(m_aSeedRcv ? m_aSeedRcv : "-"_Stz);
  sSeedRcv.vConcat(m_bSeedsSpawned[SS_SEED_RCV] ? "+"_Stz : " "_Stz);

  s32 sSeedExe(m_aSeedExe ? m_aSeedExe : "-"_Stz);
  sSeedExe.vConcat(m_bSeedsSpawned[SS_SEED_EXE] ? "+"_Stz : " "_Stz);

  s32 sSeedEnd(m_aSeedEnd ? m_aSeedEnd : "-"_Stz);
  sSeedEnd.vConcat(m_bSeedsSpawned[SS_SEED_END] ? "+"_Stz : " "_Stz);

  s32 sSeedCan(m_aSeedCan ? m_aSeedCan : "-"_Stz);
  sSeedCan.vConcat(m_bSeedsSpawned[SS_SEED_CAN] ? "+"_Stz : " "_Stz);

  vTrace("%20s %20s %10s %10s %20s : %10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         aCN[m_eClass],
         m_aName,
         sFunctions.aStr(),
         aJK[m_eKind],
         m_aComment,
         
         m_bResources ? "chk/rel"_Stz : "-"_Stz,
         
         sSeedRun.aStr(),
         sSeedReq.aStr(),
         sSeedRcv.aStr(),
         sSeedExe.aStr(),
         sSeedEnd.aStr(),
         sSeedCan.aStr()
         );

}

/*----------------------------------------------------------------------------*/
/* vtrace the header for the jobcat prompt function                           */
/*----------------------------------------------------------------------------*/
void JobClass::vJobCatHeader(void)
{
  vTrace("%20s %20s %10s %10s %20s : %10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         "eClass"_Stz,
         "Job Name"_Stz,
         "Functions"_Stz,
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
  
  vTrace("%20s %20s %10s %10s %20s : "
         "%10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         sBar20.aStr(),sBar20.aStr(),sBar10.aStr(),
         sBar10.aStr(),sBar20.aStr(),sBar10.aStr(),
         
         sBar20.aStr(),sBar20.aStr(),sBar20.aStr(),
         sBar20.aStr(),sBar20.aStr(),sBar20.aStr()
         );
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
