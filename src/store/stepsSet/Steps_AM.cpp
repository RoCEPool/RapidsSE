/*============================================================================*/
/* Steps_AM.cpp - AM step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AM(void)
{
  vAddStep("AM::DEMON::tAnswerDemonJobId@"_Stz,    &Step::vAM_AnswerDemonJobIdFetch);
  vAddStep("AM::DEMON::tAnswerDemonJobId!"_Stz,    &Step::vAM_AnswerDemonJobIdStore);

  vAddStep("AM::DEMON::tAnswerWLDemonJobId@"_Stz,  &Step::vAM_AnswerWLDemonJobIdFetch);
  vAddStep("AM::DEMON::tAnswerWLDemonJobId!"_Stz,  &Step::vAM_AnswerWLDemonJobIdStore);

  vAddStep("AM::DEMON::tCollectDemonJobId@"_Stz,   &Step::vAM_CollectDemonJobIdFetch);
  vAddStep("AM::DEMON::tCollectDemonJobId!"_Stz,   &Step::vAM_CollectDemonJobIdStore);

  vAddStep("AM::DEMON::tConnectDemonJobId@"_Stz,   &Step::vAM_ConnectorDemonJobIdFetch);
  vAddStep("AM::DEMON::tConnectDemonJobId!"_Stz,   &Step::vAM_ConnectorDemonJobIdStore);

  vAddStep("AM::DEMON::tEventDemonJobId@"_Stz,     &Step::vAM_EventDemonJobIdFetch);
  vAddStep("AM::DEMON::tEventDemonJobId!"_Stz,     &Step::vAM_EventDemonJobIdStore);

  vAddStep("AM::DEMON::tExitDemonJobId@"_Stz,      &Step::vAM_ExitDemonJobIdFetch);
  vAddStep("AM::DEMON::tExitDemonJobId!"_Stz,      &Step::vAM_ExitDemonJobIdStore);

  vAddStep("AM::DEMON::tMetricsDemonJobId@"_Stz,   &Step::vAM_MetricsDemonJobIdFetch);
  vAddStep("AM::DEMON::tMetricsDemonJobId!"_Stz,   &Step::vAM_MetricsDemonJobIdStore);

  vAddStep("AM::DEMON::tMonDemonJobId@"_Stz,       &Step::vAM_MonDemonJobIdFetch);
  vAddStep("AM::DEMON::tMonDemonJobId!"_Stz,       &Step::vAM_MonDemonJobIdStore);

  vAddStep("AM::DEMON::tNodeMonDemonJobId@"_Stz,      &Step::vAM_NodeMonDemonJobIdFetch);
  vAddStep("AM::DEMON::tNodeMonDemonJobId!"_Stz,      &Step::vAM_NodeMonDemonJobIdStore);

  vAddStep("AM::DEMON::tPageInDemonJobId@"_Stz,   &Step::vAM_PageInDemonJobIdFetch);
  vAddStep("AM::DEMON::tPageInDemonJobId!"_Stz,   &Step::vAM_PageInDemonJobIdStore);

  vAddStep("AM::DEMON::tPageOutDemonJobId@"_Stz,   &Step::vAM_PageOutDemonJobIdFetch);
  vAddStep("AM::DEMON::tPageOutDemonJobId!"_Stz,   &Step::vAM_PageOutDemonJobIdStore);

  vAddStep("AM::DEMON::tReaperDemonJobId@"_Stz,    &Step::vAM_ReaperDemonJobIdFetch);
  vAddStep("AM::DEMON::tReaperDemonJobId!"_Stz,    &Step::vAM_ReaperDemonJobIdStore);

  vAddStep("AM::DEMON::tRecurDemonJobId@"_Stz,     &Step::vAM_RecurDemonJobIdFetch);
  vAddStep("AM::DEMON::tRecurDemonJobId!"_Stz,     &Step::vAM_RecurDemonJobIdStore);

  vAddStep("AM::DEMON::tStatDemonJobId@"_Stz,      &Step::vAM_StatDemonJobIdFetch);
  vAddStep("AM::DEMON::tStatDemonJobId!"_Stz,      &Step::vAM_StatDemonJobIdStore);

  vAddStep("AM::DEMON::tWakerDemonJobId@"_Stz,     &Step::vAM_WakerDemonJobIdFetch);
  vAddStep("AM::DEMON::tWakerDemonJobId!"_Stz,     &Step::vAM_WakerDemonJobIdStore);

  vAddStep("AM::DEMON::tSuperDemonJobId@"_Stz,     &Step::vAM_SuperDemonJobIdFetch);
  vAddStep("AM::DEMON::tSuperDemonJobId!"_Stz,     &Step::vAM_SuperDemonJobIdStore);

  vAddStep("AM::MEMORY::hasMemoryLeaks"_Stz,       &Step::vAM_HasMemoryLeaks);
  vAddStep("AM::MEMORY::isNotFreeing"_Stz,         &Step::vAM_IsNotFreeing);
  vAddStep("AM::MEMORY::isTracking"_Stz,           &Step::vAM_IsTracking);
  vAddStep("AM::MEMORY::stopTracking"_Stz,         &Step::vAM_StopTracking);
  vAddStep("AM::MEMORY::leaksSnapshot"_Stz,        &Step::vAM_TakeLeaksSnapshot);
  vAddStep("AM::MEMORY::trackingSnapshot"_Stz,     &Step::vAM_TakeTrackingSnapshot);

  vAddStep("AM::SHOW::bBoot"_Stz,                  &Step::vAM_Show_Boot);
  vAddStep("AM::SHOW::bDir"_Stz,                   &Step::vAM_Show_Dir);
  vAddStep("AM::SHOW::bDir_Failure"_Stz,           &Step::vAM_Show_Dir_Failure);
  vAddStep("AM::SHOW::bConnection"_Stz,            &Step::vAM_Show_Connection);
  vAddStep("AM::SHOW::bException"_Stz,             &Step::vAM_Show_Exception);
  vAddStep("AM::SHOW::bFile"_Stz,                  &Step::vAM_Show_File);
  vAddStep("AM::SHOW::bFile_Failure"_Stz,          &Step::vAM_Show_File_Failure);
  vAddStep("AM::SHOW::bLowMem"_Stz,                &Step::vAM_Show_LowMem);
  vAddStep("AM::SHOW::bLowMem_Recv"_Stz,           &Step::vAM_Show_LowMem_Recv);
  vAddStep("AM::SHOW::bRecurring"_Stz,             &Step::vAM_Show_Recurring);
  vAddStep("AM::SHOW::bSocket"_Stz,                &Step::vAM_Show_Socket);
  vAddStep("AM::SHOW::bSocket_Failure"_Stz,        &Step::vAM_Show_Socket_Failure);

  vAddStep("AM::derefMetricsDisable"_Stz,          &Step::vAM_DerefMetricsDisable);
  vAddStep("AM::derefMetricsEnable"_Stz,           &Step::vAM_DerefMetricsEnable);

  vAddStep("AM::jobMetricsDisable"_Stz,            &Step::vAM_JobMetricsDisable);
  vAddStep("AM::jobMetricsEnable"_Stz,             &Step::vAM_JobMetricsEnable);

  vAddStep("AM::sysMetricsDisable"_Stz,            &Step::vAM_SysMetricsDisable);
  vAddStep("AM::sysMetricsEnable"_Stz,             &Step::vAM_SysMetricsEnable);

  vAddStep("AM::SYSTEM::Ready@"_Stz,         &Step::vAM_System_ReadyFetch);
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tAnswerDemonJobId@ -- fetch AnswerDemon JobId                   */
/*----------------------------------------------------------------------------*/
void Step::vAM_AnswerDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tAnswerDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tAnswerDemonJobId! -- store the AnswerDemon JobId               */
/*----------------------------------------------------------------------------*/
void Step::vAM_AnswerDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vAnswerDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tAnswerWLDemonJobId@ -- fetch AnswerWLDemon JobId               */
/*----------------------------------------------------------------------------*/
void Step::vAM_AnswerWLDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tAnswerWLDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tAnswerWLDemonJobId! -- store the AnswerWLDemon JobId           */
/*----------------------------------------------------------------------------*/
void Step::vAM_AnswerWLDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vAnswerWLDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tCollectDemonJobId@ -- fetch CollectDemon JobId                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_CollectDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tCollectDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tCollectDemonJobId! -- store the CollectDemon JobId             */
/*----------------------------------------------------------------------------*/
void Step::vAM_CollectDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vCollectDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tConnectDemonJobId@ -- fetch ConnectorDemon JobId               */
/*----------------------------------------------------------------------------*/
void Step::vAM_ConnectorDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tConnectorDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tConnectDemonJobId! -- store the ConnectorDemon JobId         */
/*----------------------------------------------------------------------------*/
void Step::vAM_ConnectorDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vConnectorDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tEventDemonJobId@ -- fetch EventDemon JobId                     */
/*----------------------------------------------------------------------------*/
void Step::vAM_EventDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tEventDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tEventDemonJobId! -- store the EventDemon JobId                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_EventDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vEventDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tExitDemonJobId@ -- fetch ExitDemon JobId                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_ExitDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tExitDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tExitDemonJobId! -- store the ExitDemon JobId                   */
/*----------------------------------------------------------------------------*/
void Step::vAM_ExitDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vExitDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tMetricsDemonJobId@ -- fetch MetricsDemon JobId                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_MetricsDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tMetricsDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tMetricsDemonJobId! -- store the MetricsDemon JobId             */
/*----------------------------------------------------------------------------*/
void Step::vAM_MetricsDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vMetricsDemonJobId(tJobId);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* AM::DEMON::tMonDemonJobId@ -- fetch MonDemon JobId                         */
/*----------------------------------------------------------------------------*/
void Step::vAM_MonDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tMonDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tMonDemonJobId! -- store the MonDemon JobId                     */
/*----------------------------------------------------------------------------*/
void Step::vAM_MonDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vMonDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tNodeMonDemonJobId@ -- fetch ExitDemon JobId                    */
/*----------------------------------------------------------------------------*/
void Step::vAM_NodeMonDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tNodeMonDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tNodeMonDemonJobId! -- store the ExitDemon JobId                */
/*----------------------------------------------------------------------------*/
void Step::vAM_NodeMonDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vNodeMonDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tPageInDemonJobId@ -- fetch PageInDemon JobId                   */
/*----------------------------------------------------------------------------*/
void Step::vAM_PageInDemonJobIdFetch(void)
{
  ENTRY();
  JobId tJobId = AM::DEMON::tPageInDemonJobId();
  m_pDataStack->vPushInt(tJobId.iAsI8());
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* AM::DEMON::tPageInDemonJobId! -- store the PageInDemon JobId             */
/*----------------------------------------------------------------------------*/
void Step::vAM_PageInDemonJobIdStore(void)
{
  ENTRY();
  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vPageInDemonJobId(tJobId);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tPageOutDemonJobId@ -- fetch PageOutDemon JobId                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_PageOutDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tPageOutDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tPageOutDemonJobId! -- store the PageOutDemon JobId             */
/*----------------------------------------------------------------------------*/
void Step::vAM_PageOutDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vPageOutDemonJobId(tJobId);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* AM::DEMON::tReaperDemonJobId@ -- fetch ReaperDemon JobId                   */
/*----------------------------------------------------------------------------*/
void Step::vAM_ReaperDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tReaperDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tReaperDemonJobId! -- store the ReaperDemon JobId               */
/*----------------------------------------------------------------------------*/
void Step::vAM_ReaperDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vReaperDemonJobId(tJobId);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* AM::DEMON::tRecurDemonJobId@ -- fetch RecurDemon JobId                     */
/*----------------------------------------------------------------------------*/
void Step::vAM_RecurDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tRecurDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tRecurDemonJobId! -- store the RecurDemon JobId                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_RecurDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vRecurDemonJobId(tJobId);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* AM::DEMON::tStatDemonJobId@ -- fetch StatDemon JobId                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_StatDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tStatDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tStatDemonJobId! -- store the StattDemon JobId                  */
/*----------------------------------------------------------------------------*/
void Step::vAM_StatDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vStatDemonJobId(tJobId);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* AM::DEMON::tWakerDemonJobId@ -- fetch WakerDemon JobId                     */
/*----------------------------------------------------------------------------*/
void Step::vAM_WakerDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tWakerDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tWakerDemonJobId! -- store the WakerDemon JobId                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_WakerDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vWakerDemonJobId(tJobId);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* AM::DEMON::tSuperDemonJobId@ -- fetch SuperDemon JobId                     */
/*----------------------------------------------------------------------------*/
void Step::vAM_SuperDemonJobIdFetch(void)
{
  ENTRY();

  JobId tJobId = AM::DEMON::tSuperDemonJobId();

  m_pDataStack->vPushInt(tJobId.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::DEMON::tSuperDemonJobId! -- store the SuperDemon JobId                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_SuperDemonJobIdStore(void)
{
  ENTRY();

  JobId tJobId = m_pDataStack->iPopInt(__func__);
  AM::DEMON::vSuperDemonJobId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::MEMORY::hasMemoryLeaks -- return true if there are memory leaks        */
/*----------------------------------------------------------------------------*/
void Step::vAM_HasMemoryLeaks(void)
{
  ENTRY();

  ObjectSnapshot* pSnapshotThen = AM::MEMORY::pSnapshotAtTrackingMemory();
  ObjectSnapshot* pSnapshotNow  = AM::MEMORY::pSnapshotAtMemoryLeaks();

  ASSERT(pSnapshotThen);
  ASSERT(pSnapshotNow);

  for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {

    eCN eClass = (eCN)i_Class;

    if (eClass == CN_FREEPOOLBUFFER || eClass == CN_HEAPBUFFER)
      continue;

    // check if this class to be ignored for memory leak detection
    if (gxaSizeHeap[eClass].m_bIgnoreMemoryLeak)
      continue;

    i8 nDelta = pSnapshotNow->m_nObjects[eClass] -
                pSnapshotThen->m_nObjects[eClass];

    if (nDelta > 0) {
      m_pDataStack->vPushBol(true);
      return;
    }
  }

  m_pDataStack->vPushBol(false);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::MEMORY::isNotFreeing -- returns true if allocations aren't being freed */
/*----------------------------------------------------------------------------*/
void Step::vAM_IsNotFreeing(void)
{
  ENTRY();

  m_pDataStack->vPushBol(gpBufferPool->bDontFree() || AM::pHeaps()->m_bNoFree);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::MEMORY::isTracking -- returns true if allocations are being tracked    */
/*----------------------------------------------------------------------------*/
void Step::vAM_IsTracking(void)
{
  ENTRY();

  ObjectSnapshot* pSnap1 = AM::MEMORY::pSnapshotAtTrackingMemory();
  ObjectSnapshot* pSnap2 = AM::MEMORY::pSnapshotAtMemoryLeaks();

  m_pDataStack->vPushBol(pSnap1 && pSnap2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::MEMORY::stopTracking -- stop tracking memory allocations               */
/*----------------------------------------------------------------------------*/
void Step::vAM_StopTracking(void)
{
  ENTRY();

  ObjectSnapshot* pSnap1 = AM::MEMORY::pSnapshotAtTrackingMemory();
  ObjectSnapshot* pSnap2 = AM::MEMORY::pSnapshotAtMemoryLeaks();
  delete pSnap1;
  delete pSnap2;

  AM::MEMORY::vSnapshotAtMemoryLeaks(nullptr);
  AM::MEMORY::vSnapshotAtTrackingMemory(nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::MEMORY::takeLeaksSnapshot -- track memory allocations                  */
/*----------------------------------------------------------------------------*/
void Step::vAM_TakeLeaksSnapshot(void)
{
  ENTRY();

  ObjectSnapshot* pSnapshotThen = AM::MEMORY::pSnapshotAtTrackingMemory();
  ObjectSnapshot* pSnapshotNow  = AM::MEMORY::pSnapshotAtMemoryLeaks();

  ASSERT(pSnapshotThen);
  ASSERT(pSnapshotNow);

  pSnapshotNow->vTakeSnapshot();
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::MEMORY::takeTrackingSnapshot -- track memory allocations               */
/*----------------------------------------------------------------------------*/
void Step::vAM_TakeTrackingSnapshot(void)
{
  ENTRY();

  if (AM::MEMORY::pSnapshotAtTrackingMemory() ||
      AM::MEMORY::pSnapshotAtMemoryLeaks()) {
    // push error string and error code on stack
    eEC eErr = EC_MEMORY_ALREADY_TRACKED;
    s2K sBuf;
    sBuf.vSprint("%s : run memoryleaks to close current analysis"_Stz, aErrorMsg[eErr]);

    m_pDataStack->vPushStz(sBuf.szStz());
    m_pDataStack->vPushInt(eErr);
    m_pDataStack->vPushBol(false);
    return;
  }

  // Create the object snapshots for both trackingmemory time and memoryleaks time
  AM::MEMORY::vSnapshotAtTrackingMemory(new ObjectSnapshot);
  AM::MEMORY::vSnapshotAtMemoryLeaks(new ObjectSnapshot);

  // Now record the trackmemory object counts
  AM::MEMORY::pSnapshotAtTrackingMemory()->vTakeSnapshot();

  i8 nJobs = AM::JOBS::nJobsActive();
  AM::JOBS::vJobsThreshold(nJobs);

  m_pDataStack->vPushBol(true);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bBoot -- <show_boot> ?                                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Boot(void)
{
  ENTRY();

  b8 bBoot = AM::SHOW::bBoot();
  
  m_pDataStack->vPushInt(bBoot);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bDir -- <show_dir> ?                                             */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Dir(void)
{
  ENTRY();

  b8 bDir = AM::SHOW::bDir();
  
  m_pDataStack->vPushInt(bDir);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bDir_Failure -- <show_dir_failure> ?                             */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Dir_Failure(void)
{
  ENTRY();

  b8 bDir_Failure = AM::SHOW::bDir_Failure();
  
  m_pDataStack->vPushInt(bDir_Failure);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bConnection -- <show_connection> ?                               */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Connection(void)
{
  ENTRY();

  b8 bConnection = AM::SHOW::bConnection();
  
  m_pDataStack->vPushInt(bConnection);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bException -- <show_exception> ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Exception(void)
{
  ENTRY();

  b8 bException = AM::SHOW::bException();
  
  m_pDataStack->vPushInt(bException);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bFile -- <show_file> ?                                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_File(void)
{
  ENTRY();

  b8 bFile = AM::SHOW::bFile();
  
  m_pDataStack->vPushInt(bFile);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bFile_Failure -- <show_file_failure> ?                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_File_Failure(void)
{
  ENTRY();

  b8 bFile_Failure = AM::SHOW::bFile_Failure();
  
  m_pDataStack->vPushInt(bFile_Failure);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bLowMem -- <show_lowmem> ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_LowMem(void)
{
  ENTRY();

  b8 bLowMem = AM::SHOW::bLowMem();
  
  m_pDataStack->vPushInt(bLowMem);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bLowMem_Recv -- <show_lowmem_recv> ?                             */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_LowMem_Recv(void)
{
  ENTRY();

  b8 bLowMem_Recv = AM::SHOW::bLowMem_Recv();
  
  m_pDataStack->vPushInt(bLowMem_Recv);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bRecurring -- <show_recurring> ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Recurring(void)
{
  ENTRY();

  b8 bRecurring = AM::SHOW::bRecurring();
  
  m_pDataStack->vPushInt(bRecurring);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bSocket -- <show_socket> ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Socket(void)
{
  ENTRY();

  b8 bSocket = AM::SHOW::bSocket();
  
  m_pDataStack->vPushInt(bSocket);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SHOW::bSocket_Failure -- <show_socket_failure> ?                       */
/*----------------------------------------------------------------------------*/
void Step::vAM_Show_Socket_Failure(void)
{
  ENTRY();

  b8 bSocket_Failure = AM::SHOW::bSocket_Failure();
  
  m_pDataStack->vPushInt(bSocket_Failure);

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* AM::derefMetricsDisable -- disable deref metrics                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_DerefMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vDerefMetrics(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::dferefMetricsEnable -- enable deref metrics                           */
/*----------------------------------------------------------------------------*/
void Step::vAM_DerefMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vDerefMetrics(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::jobMetricsDisable -- disable job metrics                              */
/*----------------------------------------------------------------------------*/
void Step::vAM_JobMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vJobMetrics(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::jobMetricsEnable -- enable job metrics                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_JobMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vJobMetrics(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::sysMetricsDisable -- disable sys metrics                               */
/*----------------------------------------------------------------------------*/
void Step::vAM_SysMetricsDisable(void)
{
  ENTRY();
  
  AM::SYSARGS::vSysMetrics(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::sysMetricsEnable -- enable sys metrics                                 */
/*----------------------------------------------------------------------------*/
void Step::vAM_SysMetricsEnable(void)
{
  ENTRY();
  
  AM::SYSARGS::vSysMetrics(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AM::SYSTEM::Ready@ -- fetch SystemReady                                    */
/*----------------------------------------------------------------------------*/
void Step::vAM_System_ReadyFetch(void)
{
  ENTRY();

  bool bSystemReady = AM::SYSTEM::bReady();

  m_pDataStack->vPushInt(bSystemReady);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
