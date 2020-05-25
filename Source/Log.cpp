/*============================================================================*/
/* Log.cpp - Log                                                              */
/*============================================================================*/

#include "Logging.h"

extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Log* Logging::_pLog(void* aAddr)
{
  Log* pLog = reinterpret_cast<Log*>(aAddr);
  
  RASSERT_TYPE(pLog,CN_LOG);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Log::operator new(size_t, Log* pLog)
{
  RASSERT(! gpLog);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Log::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Log::Log(void)
: ShmLog(CN_LOG)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  if (0 != AM::iThisNodeNo() || PsxThd::bIsJavaThread() )
    RASSERT(0);
  
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    LogEntry* pLogEntry = &m_oaEntry[i_Entry];
    pLogEntry->vZero();
  }
  
  _checkInitialised_(CN_LOG);
  
  _postcondition_(_MAGIC_,CN_LOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Log::~Log(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* -- Private Static ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new shm segment for Log                                           */
/*----------------------------------------------------------------------------*/
Log* Log::pAttachCreate(void)
{
  int iRegionFd;
  bool bResult;
  SysCall oSysCall;

  s32 sLogRegionNm((Stz*)AM::szLogRegion());
  
  
  // unlink the name of any previous segment
  IGNORE_RETURN( ShmLib::bUnlink(sLogRegionNm,oSysCall) );
  
  // open a new shm region
  bResult = ShmLib::bOpenCreate(iRegionFd,sLogRegionNm,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Log::bOpenCreate: failed to create region %s\n"_Stz,
           sLogRegionNm.azChar());
  
  // extend the shm region
  bResult = ShmLib::bExtend(iRegionFd,sizeof(Log),oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Log::bOpenCreate: failed to extend region %s\n"_Stz,
           sLogRegionNm.azChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = ShmLib::aMap(iRegionFd,
                                nullptr,
                                sizeof(Log),
                                C_NO_HUGE_PAGES,
                                oSysCall);
  if (! aSegment)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Log::bOpenCreate: failed to map region!\n"_Stz);
  
  Log* pLog = static_cast<Log*>(aSegment);
  
  s32 sLogRegionSize_magn;
  MagLib::vByteMag(sizeof(Log),sLogRegionSize_magn);

  if (AM::bVerboseProgress())
    vTrace("Created new shared log region %p: name %s size %s\n"_Stz,
           aSegment,
           sLogRegionNm.szStz(),
           sLogRegionSize_magn.szStz());

  return pLog;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm segment containing the Log                          */
/*----------------------------------------------------------------------------*/
Log* Log::pAttachExisting(void)
{
  int iFD;
  SysCall oSysCall;

  s32 sLogRegionNm((Stz*)AM::szLogRegion());

  // open existing shm region
  bool bResult = ShmLib::bOpenExisting(iFD,sLogRegionNm,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Log::bOpenExisting: failed to open region: %s\n"_Stz,
           sLogRegionNm.azChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = ShmLib::aMap(iFD,
                             nullptr,
                             sizeof(Log),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)  {
    vTrace("Log::bOpenExisting: failed to map\n"_Stz);
    ShmLib::bClose(iFD,oSysCall);
    NEVER_REACH;
  }
  
  Log* pLog = static_cast<Log*>(aSegment);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* -- Public Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a multi-line string entry                                              */
/*----------------------------------------------------------------------------*/
void Log::vNoteString(i8 iJobId, s48K& sString)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  s4K sLine;
  char* aCharAdr = sString.azChar();

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  gpLog->vGrabLock();
  
  while (StrLib::bEachLine(&aCharAdr,sLine.azChar())) {
    
    LogEntry* pLogEntry = gpLog->pNewEntry_NL();
    pLogEntry->vInit(SL_INF,CN_NONE,sLine);
    
    //pLogEntry->m_xJob.vSet(pLogEntry,iJobId,JA_NONE);
  }
  
  gpLog->vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* entry exists?                                                               */
/*----------------------------------------------------------------------------*/
bool Log::bEntryExists(i8 i_Entry)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return false;
  
  LogEntry* pLogEntry = gpLog->pGetEntry(i_Entry);
  if (! pLogEntry)
    return false;
  
  return true;
#endif
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* previous entry                                                             */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryPrev(i8 i_Entry)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return C_UNKNOWN;
  
  LogEntry* pLogEntry = gpLog->pGetEntry(i_Entry);
  if (! pLogEntry)
    return false;
  
  return pLogEntry->m_i_Prev;
#else
  return C_UNKNOWN;
#endif
  
}

/*----------------------------------------------------------------------------*/
/* entry sprint                                                               */
/*----------------------------------------------------------------------------*/
void Log::vEntrySprint(s4K& sString, i8 i_Entry)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return;

  LogEntry* pLogEntry = gpLog->pGetEntry(i_Entry);
  if (!pLogEntry) {
    sString = ""_Stz;
    return;
  }
  
  i8 iTotalDeltaUsec = 0;
  pLogEntry->vSprint(sString,0,iTotalDeltaUsec);
  
#endif
}

/*----------------------------------------------------------------------------*/
/* how was a request done?                                                    */
/*----------------------------------------------------------------------------*/
void Log::vHow(i8 iReqId, i8 iDetail)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return;
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  
  i8 i_MinEntry;
  i8 i_MaxEntry;
  vFindRange_NL(i_MinEntry,i_MaxEntry);
  
  
  LogEntry* pLogEntry = &m_oaEntry[i_MinEntry % m_nEntries];
  
  if (pLogEntry->m_xReqTuple.m_iReqNo == iReqId)
    vHow(iReqId,iDetail,pLogEntry);

  for (i8 i_Entry = i_MinEntry + 1; i_Entry <= i_MaxEntry; i_Entry++) {
    
    pLogEntry = &m_oaEntry[i_Entry % m_nEntries];
    
    if (-1 == pLogEntry->m_i_Entry)
      break;
    
    if (pLogEntry->m_xReqTuple.m_iReqNo == iReqId)
      vHow(iReqId,iDetail,pLogEntry);
  }
  

  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

#endif
}

/*----------------------------------------------------------------------------*/
/* how was a request done?                                                    */
/*----------------------------------------------------------------------------*/
void Log::vHow(i8 iReqId, i8 iDetail, LogEntry* pLogEntry)
{
  if (pLogEntry->m_xReqTuple.m_iReqNo != iReqId)
    return;
  
  if (iDetail != C_UNKNOWN) {
    if (EK_JOB != pLogEntry->m_eKind)
      return;
  }
  
  s4K sHow;
  
  pLogEntry->vAsTxt(sHow);
  
  printf((char*)"%s\n"_Stz,sHow.szStz());
}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void Log::vMonitorPortrait(s48K& sPortrait)
{
#ifdef WITH_SHARED_LOG
  if (! gpLog) {
    sPortrait.vSprint("Log: activity map not configured\n"_Stz);
    return;
  }
  
  gpLog->vGrabLock();
  i8 iPercentFull = gpLog->iPercentFull_NL();
  gpLog->vDropLock();
  
  i8 iMapSize = gpLog->iSize();
  s32 sShmMag;
  MagLib::vByteMag(iMapSize, sShmMag);
  
  s32 sArrayMag;
  MagLib::vUnitMag(gpLog->m_nEntries, sArrayMag);

  sPortrait.vSprint("Log: shm %s array %s marks %lld; %lld%% full;\n"_Stz,
                    sShmMag.szStz(),
                    sArrayMag.szStz(),
                    gpLog->m_i_Entry,
                    iPercentFull
                     );
#else
  sPortrait.vSprint("Log: not compiled WITH_SHARED_LOG\n"_Stz);
#endif
}

/*----------------------------------------------------------------------------*/
/* open Log (called by API)                                                   */
/*----------------------------------------------------------------------------*/
void Log::vOpenByApi(void)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return;
  
  if (gpLog)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Log* pLog = Log::pAttachExisting();
  if (AM::bVerbose())
    vTrace("Log %s : "
           "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
           AM::szLogRegion(),
           sizeof(Log),
           C_MAX_LOG_ENTRIES);
  
  gpLog = pLog;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::vLogLog(SL_INF,gpLog,GA_LOG_OPENED_API,
              "Opened by API"_Stz); //!format-LogLog
  
  Now oNow;
  AM::vLogLog(SL_INF,gpLog,GA_LOG_TIME,
              "Time is: "_Stz, //!format-LogLog
              oNow.sz());
#endif
}

/*----------------------------------------------------------------------------*/
/* create or open Log (called by SE)                                          */
/*----------------------------------------------------------------------------*/
void Log::vOpenBySE(bool bStarting)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return;
  
  if (gpLog)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Log::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Log* pLog = nullptr;
  
  if (bStarting) {
    pLog = Log::pAttachCreate();
    if (AM::bVerbose())
      vTrace("Log %s : "
             "created new map (size %lldB, max %lld marks)\n\n"_Stz,
             AM::szLogRegion(),
             sizeof(Log),
             C_MAX_LOG_ENTRIES);
    gpLog = new(pLog) Log;
    
    gpLog->vMarkAll(true);
   }
   else {
    pLog = Log::pAttachExisting();
    if (AM::bVerbose())
      vTrace("Log %s : "
             "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
             AM::szLogRegion(),
             sizeof(Log),
             C_MAX_LOG_ENTRIES);
    
    gpLog = pLog;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s256 sWelcome;
  AM::vWelcomeText(sWelcome);
  AM::vLogLog(SL_INF,gpLog,GA_LOG_WELCOME,
              sWelcome); //!format-LogLog
  
  s256 sCopyright;
  AM::vCopyrightText(sCopyright);
  AM::vLogLog(SL_INF,gpLog,GA_LOG_COPYRIGHT,
              sCopyright); //!format-LogLog
  
  Now oNow;
  AM::vLogLog(SL_INF,gpLog,GA_LOG_TIME,
              "Time is: %s"_Stz, //!format-LogLog
              oNow.sz());

  s32 sMem;
  MagLib::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sCpuFreq;
  MagLib::vHzMag(Hardware::iCpuFrequency(),sCpuFreq);

  s32 sHost;
  s32 sUser;
  Hardware::vHostName(sHost);
  Hardware::vLoginName(sUser);

  AM::vLogLog(SL_INF,gpLog,GA_LOG_HOST,
              "hw: host %s user %s; "
              "%lld cores; %s memory; %s clock frequency"_Stz, //!format-LogLog
             sHost.azChar(),
             sUser.azChar(),
              
             Hardware::iCoreCount(),
             sMem.szStz(),
             sCpuFreq.szStz()
             );
  
  AM::vLogArguments();
#endif
}

/*----------------------------------------------------------------------------*/
/* write Log to csv file                                                      */
/*----------------------------------------------------------------------------*/
void Log::vWriteCsv(bool bShortForm)
{
#ifdef WITH_SHARED_LOG
  if (! gpLog)
    return;
  
  gpLog->vWriteToFile(true,bShortForm);
  gpLog->vWriteToFile(false,bShortForm);
#endif
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a information log entry                                                */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryFyi(eCN eClassNo,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(SL_INF,eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xFyi.vSet(pLogEntry);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a ddl log entry                                                        */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryDdl(eSL eSeverity, Job* pJob, eDA eDdlAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pJob,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xDdl.vSet(pLogEntry,pJob,eDdlAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a job log entry                                                        */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryJob(eSL eSeverity, Job* pJob, eJA eJobAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pJob,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xJob.vSet(pLogEntry,pJob,eJobAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a lock log entry                                                       */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryLck(eSL eSeverity, Lock* pLck, eLA eLckAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pLck,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xLck.vSet(pLogEntry,pLck,eLckAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a log log entry                                                        */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryLog(eSL eSeverity, Log* pLog, eGA eLogAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pLog,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xLog.vSet(pLogEntry,pLog,eLogAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a management log entry                                                 */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryMgt(eSL eSeverity, Job* pJob, eMA eMgtAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pJob,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xMgt.vSet(pLogEntry,pJob,eMgtAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a operator log entry                                                   */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryOpr(eSL eSeverity, Op* pOpr, eOA eOprAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pOpr,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xOpr.vSet(pLogEntry,pOpr,eOprAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a prompt log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryPrm(eSL eSeverity, SePrompt* pPrm, ePA ePrmAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pPrm,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xPrm.vSet(pLogEntry,pPrm,ePrmAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a queues log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryQue(eSL eSeverity, Queues* pQue, eQA eQueAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pQue,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xQue.vSet(pLogEntry,pQue,eQueAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a resource log entry                                                   */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryRes(eSL eSeverity, Resource* pRes, eRA eResAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pRes,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xRes.vSet(pLogEntry,pRes,eResAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a api log entry                                                        */
/*----------------------------------------------------------------------------*/
i8 Log::iEntrySsn(eSL eSeverity, Session* pSsn, eAA eApiAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pSsn,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xSsn.vSet(pLogEntry,pSsn,eApiAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a service log entry                                                    */
/*----------------------------------------------------------------------------*/
i8 Log::iEntrySvc(eSL eSeverity, Job* pJob, eVA eSvcAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pJob,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xSvc.vSet(pLogEntry,pJob,eSvcAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a system log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntrySys(eSL eSeverity, AttachMap* pSys, eYA eSysAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pSys,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xSys.vSet(pLogEntry,pSys,eSysAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a thread log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryThd(eSL eSeverity, Thread* pThd, eHA eThdAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pThd,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xThd.vSet(pLogEntry,pThd,eThdAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a task log entry                                                       */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryTsk(eSL eSeverity, Task* pTsk, eTA eTskAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pTsk,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xTsk.vSet(pLogEntry,pTsk,eTskAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a thread log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryTxn(eSL eSeverity, Tx* pTxn, eXA eTxnAct,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eSeverity,pTxn,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xTxn.vSet(pLogEntry,pTxn,eTxnAct);
  
  return pLogEntry->m_i_Entry;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Log::vFocalOutline(s4K& sOutline)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}


/*----------------------------------------------------------------------------*/
/* get an entry                                                               */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pGetEntry(i8 i_Entry)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  LogEntry* pLogEntry = &m_oaEntry[i_Entry];
  
  if (i_Entry == pLogEntry->m_i_Entry)
    return pLogEntry;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a new mark in Log                                                      */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewEntry(eSL eSeverity, eCN eClassNo,
                         Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  char caText[1024];
  vsnprintf(caText,sizeof(caText)-1,szFormat->azChar(),xVaArgs);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  LogEntry* pLogEntry = pNewEntry_NL();
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pLogEntry->vInit(eSeverity,eClassNo,(Stz*)caText);
  
  //StrLib::vCopyLen(pLogEntry->m_caNote,caText,C_MAX_LOG_NOTE - 1);

  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* get a new mark in Log                                                      */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewEntry(eSL eSeverity, Focal* pFocal,
                         Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  char caText[1024];
  vsnprintf(caText,sizeof(caText)-1,szFormat->azChar(),xVaArgs);
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  LogEntry* pLogEntry = pNewEntry_NL();
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pLogEntry->vInit(eSeverity,pFocal,(Stz*)caText);
  
  //StrLib::vCopyLen(pLogEntry->m_caNote,caText,C_MAX_LOG_NOTE - 1);
  
  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* get a new mark in Log (non-locking)                                        */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewEntry_NL(void)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  m_okLock.vAssertWrite();
  
  LogEntry* pLogEntry = &m_oaEntry[m_i_Entry % m_nEntries];
  
  pLogEntry->m_i_Entry = m_i_Entry;
  
  m_i_Entry++;
  
  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* find active range                                                          */
/*----------------------------------------------------------------------------*/
void Log::vFindRange_NL(i8& i_MinEntry, i8& i_MaxEntry)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  // should be a quicker way to do this
  
  i_MinEntry = C_I8_MAX;
  i_MaxEntry = C_I8_MIN;
  
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    
    if (-1 == m_oaEntry[i_Entry].m_i_Entry)
      return;
    
    if (m_oaEntry[i_Entry].m_i_Entry < i_MinEntry)
      i_MinEntry = m_oaEntry[i_Entry].m_i_Entry;
    
    if (m_oaEntry[i_Entry].m_i_Entry > i_MaxEntry)
      i_MaxEntry = m_oaEntry[i_Entry].m_i_Entry;
  }
}

/*----------------------------------------------------------------------------*/
/* calculate how full the log is                                              */
/*----------------------------------------------------------------------------*/
i8 Log::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  i8 nEntriesInUse = 0;
  i8 iPercent = 0;
  
  i8 i_MinEntry;
  i8 i_MaxEntry;
  vFindRange_NL(i_MinEntry,i_MaxEntry);
  
  if (i_MinEntry < i_MaxEntry) {
    //
    // Example: 0 .. 25 __ [64]
    //          i_MinEntry is 0
    //          i_MaxEntry is 48
    //          m_nEntries  is 64
    // then only (0..25) is in use out of 64
    //
    nEntriesInUse = (i_MaxEntry - i_MinEntry);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_MinEntry is 48
    //          i_MaxEntry is 25
    //          m_nEntries  is 64
    // then (48..63) + (0..25) is in use
    //
    nEntriesInUse = (m_nEntries - i_MinEntry) + (i_MaxEntry);
  }
  
  iPercent = (nEntriesInUse * 100) / m_nEntries;
  
  return iPercent;
}

/*----------------------------------------------------------------------------*/
/* compute the size of Log given gpAttachMap->m_nActivityMarks                */
/*----------------------------------------------------------------------------*/
i8 Log::iSize(void)
{
  i8 iMapSize = sizeof(Log);
  
  return iMapSize;
}

/*----------------------------------------------------------------------------*/
/* write Log to a csv file                                                    */
/*----------------------------------------------------------------------------*/
void Log::vWriteToFile(bool bCsv, bool bShortForm)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  s256 sLogPath;
  sLogPath = bCsv ? (Stz*)AM::szLogCsvFile() : (Stz*)AM::szLogTxtFile();

  File* pLogFile = new File(nullptr);
  
  bool bOpened = pLogFile->bOpenWrite(sLogPath);
  if (! bOpened) {
    vTrace("Unable to open log file: %s\n"_Stz,sLogPath.azChar());
    delete pLogFile;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Now oNow;
  AM::vLogLog(SL_INF,gpLog,GA_LOG_WRITING,
              "Writing Log at: %s"_Stz, //!format-LogLog
              oNow.sz());
  
  s256 sWelcome;
  AM::vWelcomeText(sWelcome);
  AM::vLogLog(SL_INF,gpLog,GA_LOG_WELCOME,
              sWelcome); //!format-LogLog
  
  s256 sCopyright;
  AM::vWelcomeText(sCopyright);
  AM::vLogLog(SL_INF,gpLog,GA_LOG_COPYRIGHT,
              sCopyright); //!format-LogLog

  i8 iUpTimeSsec = AM::iElapsedSsec();
  AM::vLogLog(SL_INF,gpLog,GA_LOG_UPTIME,
              "Uptime: %llds; Requests: %lld"_Stz, //!format-LogLog
              iUpTimeSsec,C_UNKNOWN);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_MinEntry;
  i8 i_MaxEntry;
  i8 iTotalDeltaUsec = 0;
  
  vFindRange_NL(i_MinEntry,i_MaxEntry);
  
  LogEntry* pLogEntry = &m_oaEntry[i_MinEntry % m_nEntries];
  
  if (bCsv)
    pLogEntry->vWriteCsv(pLogFile,0,iTotalDeltaUsec);
  else
    pLogEntry->vWriteTxt(pLogFile);

  for (i8 i_Entry = i_MinEntry + 1; i_Entry <= i_MaxEntry; i_Entry++) {
    
    pLogEntry = &m_oaEntry[i_Entry % m_nEntries];
    
    if (-1 == pLogEntry->m_i_Entry)
      break;
    
    LogEntry* pPrevEntry = &m_oaEntry[(i_Entry - 1) % m_nEntries];
    i8 iEntryDeltaUsec = TickingDevice::iToUsec(pLogEntry->m_iTick - pPrevEntry->m_iTick);

    if (bCsv)
      pLogEntry->vWriteCsv(pLogFile,iEntryDeltaUsec,iTotalDeltaUsec);
    else
      pLogEntry->vWriteTxt(pLogFile);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pLogFile->vClose();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("Log written to: %s\n"_Stz,sLogPath.azChar());
  
  delete pLogFile;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* process log command                                                        */
/*----------------------------------------------------------------------------*/
void Log::vLog(TokenVec* pTknVec)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  i8 nTokens = pTknVec->nItems();
  
  if (1 == nTokens) {
    vLogList(pTknVec);
    return;
  }
  
  if (3 == nTokens && pTknVec->pGet(1)->bSame("mark"_Stz)) {
    vLogMark(pTknVec);
    return;
  }

  vTrace("Unknown Log\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* list all focal objects                                                     */
/*----------------------------------------------------------------------------*/
void Log::vLogList(TokenVec* pTknVec)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  vTrace(" Log list ???\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* mark classes                                                               */
/*----------------------------------------------------------------------------*/
void Log::vLogMark(TokenVec* pTknVec)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  i8 nTokens = pTknVec->nItems();
  
  if (3 == nTokens && pTknVec->pGet(2)->bSame("all"_Stz)) {
    EnumFlags* pClassFlags = AM::pClassFlags();
    pClassFlags->vAll(true);
    return;
  }
  
  vTrace(" Log mark ???\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* mark all                                                                   */
/*----------------------------------------------------------------------------*/
void Log::vMarkAll(bool bAll)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  EnumFlags* pClassFlags = AM::pClassFlags();
  
  pClassFlags->vAll(bAll);
}

/*----------------------------------------------------------------------------*/
/* is loggable ?                                                              */
/*----------------------------------------------------------------------------*/
bool Log::bIsLoggable(Object* pObject)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  if (! pObject)
    return false;
  
  EnumFlags* pClassFlags = AM::pClassFlags();
  
  eCN eClassNo = pObject->eClass();
  
  if (pClassFlags->bFlagged(eClassNo))
    return true;
  
  return false;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Log);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
