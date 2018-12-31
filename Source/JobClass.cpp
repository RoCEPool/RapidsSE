/*============================================================================*/
/* JobClass.cpp -- a description of a job class                               */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::_pJobClass(i8 iAddr)
{
  JobClass* pJobClass = reinterpret_cast<JobClass*>(iAddr);
  
  return pJobClass;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::_pJobClass(void* aAddr)
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
void JobClass::vDeleteJob(Interface::Session* pSession, Job* pJob)
{
  (*m_fDelete)(pSession, pJob);
}

/*----------------------------------------------------------------------------*/
/* end the job by running the post-function                                   */
/*----------------------------------------------------------------------------*/
void JobClass::vPostJob(Interface::Session* pSession, Job* pJob)
{
  (*m_fPost)(pSession, pJob);
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the pre-function                                */
/*----------------------------------------------------------------------------*/
Job* JobClass::pPreJob(Interface::Session* pSession)
{
  Job* pJob = (*m_fPre)(pSession);
  
  pJob->m_pJobClass = this;
  pJob->m_eJobTrace = m_eTrace;
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* get seed                                                                   */
/*----------------------------------------------------------------------------*/
Stz* JobClass::sSeed(eSS eSeed)
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
  
  vTrace("%20s %10lld %20s %10s %10s %20s : %10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         aCN[m_eClass],
         giClassSize[m_eClass],
         m_aName,
         sFunctions.aStz(),
         aJK[m_eKind],
         m_aComment,
         
         m_bResources ? "chk/rel"_Stz : "-"_Stz,
         
         sSeedRun.aStz(),
         sSeedReq.aStz(),
         sSeedRcv.aStz(),
         sSeedExe.aStz(),
         sSeedEnd.aStz(),
         sSeedCan.aStz()
         );
  
}

/*----------------------------------------------------------------------------*/
/* vtrace the header for the jobcat prompt function                           */
/*----------------------------------------------------------------------------*/
void JobClass::vJobCatHeader(void)
{
  vTrace("%20s %10s %20s %10s %10s %20s : %10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         "eClass"_Stz,
         "SizeOf"_Stz,
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
  
  vTrace("%20s %10s %20s %10s %10s %20s : "
         "%10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         sBar20.aStz(),sBar10.aStz(),sBar20.aStz(),sBar10.aStz(),
         sBar10.aStz(),sBar20.aStz(),sBar10.aStz(),
         
         sBar20.aStz(),sBar20.aStz(),sBar20.aStz(),
         sBar20.aStz(),sBar20.aStz(),sBar20.aStz()
         );
  
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
pJOBCLASSFUN JobClass::pFirstFun(void)
{
  return (pJOBCLASSFUN)pJob__Begun_class;
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
JobClass* JobClass::pFind(eCN eClass)
{
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob__Begun_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if (pJobClass->m_eClass == eClass)
      return pJobClass;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find job class given command                                               */
/*----------------------------------------------------------------------------*/
JobClass* JobClass::pFind(Stz* aCommand1, Stz* aCommand2)
{
  //vTrace("FindJob: %s %s\n"_Stz, aCommand1, aCommand2);
  
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob__Begun_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if ( ! pJobClass->m_aCommand1 ||
        ! StrLib::bSame(pJobClass->m_aCommand1->aChar(),aCommand1->aChar()) )
      continue;
    
    //if (aCommand2->iLength()) {
    if (pJobClass->m_aCommand2) {
      if ( ! StrLib::iLength(pJobClass->m_aCommand2->aChar()) )
        continue;
      if ( ! StrLib::bSame(pJobClass->m_aCommand2->aChar(),aCommand2->aChar()) )
        continue;
    }
    
    if (pJobClass->m_bEnabled)
      return pJobClass;
    else
      return nullptr;
    
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* trace the job for the given class                                          */
/*----------------------------------------------------------------------------*/
void JobClass::vTraceJobs(eCN eClass)
{
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = pFirstFun(); pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if (pJobClass->m_eClass == eClass)
      pJobClass->vJobCat();
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
