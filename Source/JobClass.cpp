/*============================================================================*/
/* JobClass.cpp -- a description of a job class                               */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass_s* JobsModel::_pJobClass_s(i8 iAddr)
{
  JobClass_s* pJobClass_s = reinterpret_cast<JobClass_s*>(iAddr);
  
  return pJobClass_s;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass_s* JobsModel::_pJobClass_s(void* aAddr)
{
  JobClass_s* pJobClass_s = reinterpret_cast<JobClass_s*>(aAddr);
  
  return pJobClass_s;
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the new-function                               */
/*----------------------------------------------------------------------------*/
Job* JobClass_s::pNewJob(Job* pArg)
{
  Job* pNewJob = (*m_fNew)(pArg);
  
  pNewJob->m_pJobClass_s = this;
  pNewJob->m_eJobTrace = m_eTrace;
  
  return pNewJob;
}

/*----------------------------------------------------------------------------*/
/* delete the job by running the delete-function                              */
/*----------------------------------------------------------------------------*/
void JobClass_s::vDeleteJob(Interface::SEitf* pSEitf, Job* pJob)
{
  (*m_fDelete)(pSEitf, pJob);
}

/*----------------------------------------------------------------------------*/
/* end the job by running the post-function                                   */
/*----------------------------------------------------------------------------*/
void JobClass_s::vPostJob(Interface::SEitf* pSEitf, Job* pJob)
{
  (*m_fPost)(pSEitf, pJob);
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the pre-function                                */
/*----------------------------------------------------------------------------*/
Job* JobClass_s::pPreJob(Interface::SEitf* pSEitf)
{
  Job* pJob = (*m_fPre)(pSEitf);
  
  pJob->m_pJobClass_s = this;
  pJob->m_eJobTrace = m_eTrace;
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* get seed                                                                   */
/*----------------------------------------------------------------------------*/
Stz* JobClass_s::sSeed(eSS eSeed)
{
  switch (eSeed) {
    case SS_SEED_RUN: return (Stz*)m_aSeedRun; break;
    case SS_SEED_REQ: return (Stz*)m_aSeedReq; break;
    case SS_SEED_RCV: return (Stz*)m_aSeedRcv; break;
    case SS_SEED_EXE: return (Stz*)m_aSeedExe; break;
    case SS_SEED_END: return (Stz*)m_aSeedEnd; break;
    case SS_SEED_CAN: return (Stz*)m_aSeedCan; break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* pre-send function                                                          */
/*----------------------------------------------------------------------------*/
void JobClass_s::vSendJob(Job* pJob)
{
  if (m_fSend)
    (*m_fSend)(pJob);
}

/*----------------------------------------------------------------------------*/
/* post-receive fucntion                                                      */
/*----------------------------------------------------------------------------*/
void JobClass_s::vRecvJob(Job* pJob)
{
  if (m_fRecv)
    (*m_fRecv)(pJob);
}

/*----------------------------------------------------------------------------*/
/* show the job                                                               */
/*----------------------------------------------------------------------------*/
void JobClass_s::vShowJob(Job* pJob)
{
  (*m_fShow)(pJob);
}

/*----------------------------------------------------------------------------*/
/* vtrace the job class for the jobcat prompt function                        */
/*----------------------------------------------------------------------------*/
void JobClass_s::vJobCat(void)
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
void JobClass_s::vJobCatHeader(void)
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

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
pJOBCLASSFUN JobClass_s::pFirstFun(void)
{
  return (pJOBCLASSFUN)pJob__Begun_class;
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
JobClass_s* JobClass_s::pFindJobClass_s(eCN eClass)
{
  JobClass_s* pJobClass_s = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob__Begun_class; pfJobClass; ) {
    
    pJobClass_s = pfJobClass((pFUN**)&pfJobClass);
    
    if (pJobClass_s->m_eClass == eClass)
      return pJobClass_s;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass_s* JobClass_s::pFindJobClass_s(Stz* aCommand1, Stz* aCommand2)
{
  //vTrace("FindJob: %s %s\n"_Stz, aCommand1, aCommand2);
  
  JobClass_s* pJobClass_s = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob__Begun_class; pfJobClass; ) {
    
    pJobClass_s = pfJobClass((pFUN**)&pfJobClass);
    
    if ( ! pJobClass_s->m_aCommand1 ||
        ! StrLib::bSame(pJobClass_s->m_aCommand1->aChar(),aCommand1->aChar()) )
      continue;
    
    //if (aCommand2->iLength()) {
    if (pJobClass_s->m_aCommand2) {
      if ( ! StrLib::iLength(pJobClass_s->m_aCommand2->aChar()) )
        continue;
      if ( ! StrLib::bSame(pJobClass_s->m_aCommand2->aChar(),aCommand2->aChar()) )
        continue;
    }
    
    if (pJobClass_s->m_bEnabled)
      return pJobClass_s;
    else
      return nullptr;
    
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* trace the job for the given class                                          */
/*----------------------------------------------------------------------------*/
void JobClass_s::vTraceJobs_s(eCN eClass)
{
  JobClass_s* pJobClass_s = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = pFirstFun(); pfJobClass; ) {
    
    pJobClass_s = pfJobClass((pFUN**)&pfJobClass);
    
    if (pJobClass_s->m_eClass == eClass)
      pJobClass_s->vJobCat();
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
