/*============================================================================*/
/* JobClass.cpp -- a description of a job class                               */
/*============================================================================*/

#include "Jobs.h"

extern SizeHeap  gxaSizeHeap[];

extern JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::_pJobClass(i8 iAddr)
{
  JobClass* pJobClass = reinterpret_cast<JobClass*>(iAddr);
  
  return pJobClass;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::_pJobClass(void* aAddr)
{
  JobClass* pJobClass = reinterpret_cast<JobClass*>(aAddr);
  
  return pJobClass;
}


/*----------------------------------------------------------------------------*/
/* post-receive fucntion                                                      */
/*----------------------------------------------------------------------------*/
eEC JobClass::eCheckJob(void)
{
  if (! m_fpCheck)
    return EC_NONE;

  eEC eErrorCode = (*m_fpCheck)();

  return eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* vtrace the job class for the jobcat prompt function                        */
/*----------------------------------------------------------------------------*/
void JobClass::vCmds(void)
{
  if (m_aHelp)
    vTrace("%7s %s\n"_Stz,aJK[m_eKind],m_aHelp);
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the new-function                               */
/*----------------------------------------------------------------------------*/
Job* JobClass::pNewJob(ReqTuple* aReqTuple)
{
  Job* pNewJob = (*m_fpNew)(this,aReqTuple);
  
  pNewJob->vJobClass(this,(char*)__func__);
  
  return pNewJob;
}

/*----------------------------------------------------------------------------*/
/* delete the job by running the delete-function                              */
/*----------------------------------------------------------------------------*/
void JobClass::vDeleteJob(Job* pJob)
{
  (*m_fpDelete)(pJob);
}

/*----------------------------------------------------------------------------*/
/* drop ddl lock                                                              */
/*----------------------------------------------------------------------------*/
void JobClass::vDropDdlLock(void)
{
  switch (m_eDdlLock) {
    case DL_NONE:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "No ddl lock specified for job class: %s\n"_Stz,aCN[m_eClass]);
      
    case DL_GRAB_READ:
      AM::pDdlLock()->vDropRead();
      break;
      
    case DL_GRAB_WRITE:
      AM::pDdlLock()->vDropWrite();
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* grab ddl lock                                                              */
/*----------------------------------------------------------------------------*/
void JobClass::vGrabDdlLock(void)
{
  switch (m_eDdlLock) {
    case DL_NONE:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "No ddl lock specified for job class: %s\n"_Stz,aCN[m_eClass]);
      
    case DL_GRAB_READ:
      AM::pDdlLock()->vGrabRead();
      break;
      
    case DL_GRAB_WRITE:
      AM::pDdlLock()->vGrabWrite();
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* end the job by running the post-function                                   */
/*----------------------------------------------------------------------------*/
void JobClass::vPostJob(Job* pJob)
{
  (*m_fpPost)(this, pJob);
}

/*----------------------------------------------------------------------------*/
/* create a new job by running the pre-function                               */
/*----------------------------------------------------------------------------*/
Job* JobClass::pPreJob(ReqTuple* aReqTuple, TagSet* pTagSet)
{
  // create the job
  
  Job* pJob = (*m_fpPre)(this,aReqTuple,pTagSet);
  
  //pJob->m_xReqTuple.m_i_Session = -1;
  
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
  if (m_fpSend)
    (*m_fpSend)(pJob);
}

/*----------------------------------------------------------------------------*/
/* post-receive fucntion                                                      */
/*----------------------------------------------------------------------------*/
void JobClass::vRecvJob(Job* pJob)
{
  if (m_fpRecv)
    (*m_fpRecv)(pJob);
}

/*----------------------------------------------------------------------------*/
/* show the job                                                               */
/*----------------------------------------------------------------------------*/
void JobClass::vShowJob(Job* pJob)
{
  pJob->vShow();
  
  if (m_fpShow)
    (*m_fpShow)(pJob);
}

/*----------------------------------------------------------------------------*/
/* vtrace the job class for the jobcat prompt function                        */
/*----------------------------------------------------------------------------*/
void JobClass::vJobCat(void)
{
  s32 sDdlLock("-"_Stz);
  
  if (DL_NONE != m_eDdlLock)
    sDdlLock = aDL[m_eDdlLock];

  s32 sFlags("-"_Stz);
  if (JF_LOWMEM == m_eFlags)
    sFlags = aJF[JF_LOWMEM];

  s32 sFunctions(""_Stz);

  sFunctions.vConcat(m_fpCheck  ? "c"_Stz : " "_Stz);
  sFunctions.vConcat(m_fpPre    ? "p"_Stz : " "_Stz);
  sFunctions.vConcat(m_fpPost   ? "P"_Stz : " "_Stz);
  sFunctions.vConcat(m_fpNew    ? "n"_Stz : " "_Stz);
  sFunctions.vConcat(m_fpDelete ? "d"_Stz : " "_Stz);
  sFunctions.vConcat(m_fpSend   ? "s"_Stz : " "_Stz);
  sFunctions.vConcat(m_fpRecv   ? "r"_Stz : " "_Stz);
  sFunctions.vConcat(m_fpShow   ? "."_Stz : " "_Stz);
  
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
  
  vTrace("%20s %10lld %10s %10s %10s %10s %10s %20s : "
         "%10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         aCN[m_eClass],
         gxaSizeHeap[m_eClass].m_nClassSize,
         sFunctions.szStz(),
         aJK[m_eKind],
         sDdlLock.szStz(),
         aPY[m_ePriority],
         sFlags.szStz(),
         m_aComment,
         
         m_bResources ? "chk/rel"_Stz : "-"_Stz,
         sSeedRun.szStz(),
         sSeedReq.szStz(),
         sSeedRcv.szStz(),
         sSeedExe.szStz(),
         sSeedEnd.szStz(),
         sSeedCan.szStz()
         );
  
}

/*----------------------------------------------------------------------------*/
/* vtrace the header for the jobcat prompt function                           */
/*----------------------------------------------------------------------------*/
void JobClass::vJobCatHeader(void)
{
  vTrace("%20s %10s %10s %10s %10s %10s %10s %20s : %10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         "eClass"_Stz,
         "SizeOf"_Stz,
         "Functions"_Stz,
         "Kind"_Stz,
         "Ddl Lock"_Stz,
         "Priority"_Stz,
         "Flags"_Stz,
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
  
  vTrace("%20s %10s %10s %10s %10s %10s %10s %20s : "
         "%10s %20s %20s %20s %20s %20s %20s\n"_Stz,
         
         sBar20.szStz(),sBar10.szStz(),sBar10.szStz(),
         sBar10.szStz(),sBar10.szStz(),sBar10.szStz(),sBar10.szStz(),sBar20.szStz(),
         
         sBar10.szStz(),sBar20.szStz(),sBar20.szStz(),sBar20.szStz(),
         sBar20.szStz(),sBar20.szStz(),sBar20.szStz()
         );
  
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
pJOBCLASSFUN JobClass::pFirstFun(void)
{
  return (pJOBCLASSFUN)pJob_Distributed_class;
}

/*----------------------------------------------------------------------------*/
/* trace the job for the given class                                          */
/*----------------------------------------------------------------------------*/
void JobClass::vCmds(eCN eClass)
{
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = pFirstFun(); pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if (pJobClass->m_eClass == eClass)
    pJobClass->vCmds();
  }
}

/*----------------------------------------------------------------------------*/
/* find job class given class number                                          */
/*----------------------------------------------------------------------------*/
JobClass* JobClass::pFind(eCN eClass)
{
  JobClass* pJobClass = nullptr;
  
  pJobClass = gxJobCatalog.aFindClass(eClass);
  if (pJobClass)
    return pJobClass;

  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob_Distributed_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    //vTrace("class: %s\n"_Stz,aCN[ pJobClass->m_eClass]);
    
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
  
  pJobClass = gxJobCatalog.aFindClass(aCommand1, aCommand2);
  if (pJobClass)
    return pJobClass;
  

  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob_Distributed_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if ( ! pJobClass->m_aCommand1 ||
        ! StrLib::bSame(pJobClass->m_aCommand1->azChar(),aCommand1->azChar()) )
      continue;
    
    //if (aCommand2->iLength()) {
    if (pJobClass->m_aCommand2) {
      if ( ! StrLib::nLength(pJobClass->m_aCommand2->azChar()) )
        continue;
      if ( ! StrLib::bSame(pJobClass->m_aCommand2->azChar(),aCommand2->azChar()) )
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
void JobClass::vJobCat(eCN eClass)
{
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = pFirstFun(); pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if (pJobClass->m_eClass == eClass)
      pJobClass->vJobCat();
  }
}

/*----------------------------------------------------------------------------*/
/* verify all job seeds                                                       */
/*----------------------------------------------------------------------------*/
void JobClass::vVerifySeeds(void)
{
  _static_
  
  //vTrace("FindJob: %s %s\n"_Stz, aCommand1, aCommand2);
  
  Dic* pRootDic = AM::pRootDic();
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = (pJOBCLASSFUN)pJob_Distributed_class; pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);

    //vTrace("Job: %s\n"_Stz, aCN[pJobClass->m_eClass]);
    
//    vTrace("%s\n"_Stz,pJobClass->m_aName);
//
//    if (pJobClass->m_aSeedRun) vTrace("  %s\n"_Stz,pJobClass->m_aSeedRun);
//    if (pJobClass->m_aSeedReq) vTrace("  %s\n"_Stz,pJobClass->m_aSeedReq);
//    if (pJobClass->m_aSeedRcv) vTrace("  %s\n"_Stz,pJobClass->m_aSeedRcv);
//    if (pJobClass->m_aSeedExe) vTrace("  %s\n"_Stz,pJobClass->m_aSeedExe);
//    if (pJobClass->m_aSeedEnd) vTrace("  %s\n"_Stz,pJobClass->m_aSeedEnd);
//    if (pJobClass->m_aSeedCan) vTrace("  %s\n"_Stz,pJobClass->m_aSeedCan);

    if (pJobClass->m_aSeedRun &&
        ! pRootDic->pFindWord(pJobClass->m_aSeedRun))
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Cannot find class %s run seed %s\n"_Stz,
             aCN[pJobClass->m_eClass],pJobClass->m_aSeedRun);

    if (pJobClass->m_aSeedReq &&
        ! pRootDic->pFindWord(pJobClass->m_aSeedReq))
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Cannot find class %s req seed %s\n"_Stz,
             aCN[pJobClass->m_eClass],pJobClass->m_aSeedReq);
    
    if (pJobClass->m_aSeedRcv &&
        ! pRootDic->pFindWord(pJobClass->m_aSeedRcv))
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Cannot find class %s rcv seed %s\n"_Stz,
             aCN[pJobClass->m_eClass],pJobClass->m_aSeedRcv);
    
    if (pJobClass->m_aSeedExe &&
        ! pRootDic->pFindWord(pJobClass->m_aSeedExe))
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Cannot find class %s exe seed %s\n"_Stz,
             aCN[pJobClass->m_eClass],pJobClass->m_aSeedExe);
    
    if (pJobClass->m_aSeedEnd &&
        ! pRootDic->pFindWord(pJobClass->m_aSeedEnd))
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Cannot find class %s end seed %s\n"_Stz,
             aCN[pJobClass->m_eClass],pJobClass->m_aSeedEnd);
    
    if (pJobClass->m_aSeedCan &&
        ! pRootDic->pFindWord(pJobClass->m_aSeedCan))
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
            "Cannot find class %s can seed %s\n"_Stz,
             aCN[pJobClass->m_eClass],pJobClass->m_aSeedCan);
    
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
