/*============================================================================*/
/* Log.cpp - System Log                                                       */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

Log* gpLog = nullptr;


/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  if (0 != gpAttachMap->m_pGrid->m_iThisNode || Pthread::bIsJavaThread() )
    RASSERT(0);
  
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    LogEntry* pLogEntry = &m_oaEntry[i_Entry];
    pLogEntry->vInit();
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
/* create and attach to a new shm region containing the Log                   */
/*----------------------------------------------------------------------------*/
Log* Log::pAttachCreate(void)
{
  int iRegionFd;
  SysCall oSysCall;
  s32 sRegionName;
  bool bResult;
  
  sRegionName = *C_LOG_REGION;
  
  // unlink the name of any previous segment
  IGNORE_RETURN( Shm::bUnlink(sRegionName,oSysCall) );
  
  // open a new shm region which will contain the shared log
  bResult = Shm::bOpenCreate(sRegionName,iRegionFd,oSysCall);
  if (! bResult)
    vFatal("Log::pAttachCreate: failed to create region %s\n"_Stz,
           sRegionName.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,sizeof(Log),oSysCall);
  if (! bResult)
    vFatal("Log::pAttachCreate: failed to extend region %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             sizeof(Log),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("Log::pAttachCreate: failed to map region!\n"_Stz);
  
  Log* pLog = static_cast<Log*>(aSegment);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm region containing the Log                           */
/*----------------------------------------------------------------------------*/
Log* Log::pAttachExisting(void)
{
  int iFD;
  SysCall oSysCall;
  s32 sRegionName;
  
  sRegionName = *C_LOG_REGION;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if (! bResult)
    vFatal("Log::pAttachExisting: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             sizeof(Log),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)  {
    vTrace("Log::pAttachExisting: failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    NEVER_REACH;
  }
  
  Log* pLog = static_cast<Log*>(aSegment);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* -- Public Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a log entry for an api call                                            */
/*----------------------------------------------------------------------------*/
void Log::vMarkApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
       ! gpAttachMap->m_bMarkApiAll )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpLog->iAddMarkArg(C_JOB_GROUP,iJobId,bForeign,aFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for api entry                                              */
/*----------------------------------------------------------------------------*/
void Log::vMarkApiEntry(i8 eApiCall, i8 iJobId)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpLog->iAddMarkVbl(C_JOB_GROUP,iJobId,bForeign,">Api %s"_Stz,zApiName);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for api error                                              */
/*----------------------------------------------------------------------------*/
void Log::vMarkApiError(i8 eApiCall, i8 iJobId, i8 iError)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpLog->iAddMarkVbl(C_JOB_GROUP,iJobId,bForeign,"!Api %s Error %lld"_Stz,zApiName,iError);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for api exit                                               */
/*----------------------------------------------------------------------------*/
void Log::vMarkApiExit(i8 eApiCall, i8 iJobId)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpLog->iAddMarkVbl(C_JOB_GROUP,iJobId,bForeign,"<Api %s"_Stz,zApiName);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a general log entry                                                    */
/*----------------------------------------------------------------------------*/
void Log::vMarkInfo(Stz* aFormat, ...)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  gpLog->iAddMarkArg(C_INF_GROUP,0,false,aFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a job-specific log entry                                               */
/*----------------------------------------------------------------------------*/
void Log::vMarkJob(Jobs::Job* pJob, Stz* szFormat, ...)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  s4K szCombinedFormat;

  if (pJob->m_bWasReceived) {
    
    s32 sNodeNm;
    sNodeNm = pJob->szNodeNm();
    
    StrLib::vSprint(szCombinedFormat,
                  "%s (recv from %s)"_Stz,
                  
                  szFormat,
                  sNodeNm.aStz()
                  );
  }
  else {
    StrLib::vSprint(szCombinedFormat,
                    "%s"_Stz,
                    
                    szFormat
                    );
  }
  
  i8 iJobId = pJob->iJobId();
  bool bForeign = pJob->m_bWasReceived;
  pJob->m_i_Activity = gpLog->iAddMarkArg(C_JOB_GROUP,iJobId,bForeign,
                                                  szCombinedFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a resource specific log entry                                          */
/*----------------------------------------------------------------------------*/
void Log::vMarkRes(eSR eSysRes,Stz* szFormat, ...)
{

#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkResource[eSysRes] &&
       ! gpAttachMap->m_bMarkResourceAll )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  gpLog->iAddMarkArg(_EG_SR,eSysRes,false,szFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry section marker                                             */
/*----------------------------------------------------------------------------*/
void Log::vMarkSection(Stz* szSectionNm)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;

  char caSection[256];
  
  i8 iSectNmLen = szSectionNm->iLength();
    
  StrLib::vPad(caSection,127-iSectNmLen-4,'#');
    
  StrLib::vConcat(caSection,' ');
  StrLib::vConcat(caSection,szSectionNm->aChar());
  StrLib::vConcat(caSection,' ');
  StrLib::vConcat(caSection,'#');
  StrLib::vConcat(caSection,'#');
    
  vMarkInfo((Stz*)caSection);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a multi-line string log entry                                          */
/*----------------------------------------------------------------------------*/
void Log::vMarkString(i8 iJobId, s48K& sString)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  s4K sLine;
  char* aCharAdr = sString.aChar();

  bool bForeign = Job::bIsForeign(iJobId);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  gpLog->vGrabLock();
  
  while (StrLib::bEachLine(&aCharAdr,sLine.aChar())) {
    //vTrace("Line: ]]%s[[\n"_Stz,sLine.aChar());
    LogEntry* pLogEntry = gpLog->pNewMark_NL();
    pLogEntry->vWrite(C_JOB_GROUP,iJobId,bForeign,sLine.aChar());
  }
  
  gpLog->vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

#endif
}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void Log::vMonitorPortrait(s48K& sPortrait)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpLog) {
    sPortrait.vSprint("Log: shared log not configured\n"_Stz);
    return;
  }
  
  gpLog->vGrabLock();
  i8 iPercentFull = gpLog->iPercentFull_NL();
  gpLog->vDropLock();
  
  i8 iMapSize = gpLog->iSize();
  s32 sShmMag;
  Magnitudes::vByteMag(iMapSize, sShmMag);
  
  s32 sArrayMag;
  Magnitudes::vUnitMag(gpLog->m_nEntries, sArrayMag);

  sPortrait.vSprint("Log: shm %s array %s marks %lld; %lld%% full;\n"_Stz,
                    sShmMag.aStz(),
                    sArrayMag.aStz(),
                    gpLog->m_i_Entry,
                    iPercentFull
                     );
#else
  sPortrait.vSprint("Log: not compiled WITH_ACTIVITY_MAP\n"_Stz);
#endif
}

/*----------------------------------------------------------------------------*/
/* open Log (called by API)                                                   */
/*----------------------------------------------------------------------------*/
void Log::vOpenByApi(void)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpAttachMap->Arg.m_bLog)
    return;
  
  if (gpLog)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Log* pLog = Log::pAttachExisting();
  if (gpAttachMap->m_bVerbose)
    vTrace("Log %s : "
           "attached to existing shared log (size %lldB, max %lld marks)\n\n"_Stz,
           C_LOG_REGION,
           sizeof(Log),
           C_LOG_ENTRIES);
  
  gpLog = pLog;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Log::vMarkInfo("Opened by API"_Stz);
  
  Now oNow;
  Log::vMarkInfo("Time is: "_Stz,oNow.sz());
#endif
}

/*----------------------------------------------------------------------------*/
/* create or open Log (called by SE)                                          */
/*----------------------------------------------------------------------------*/
void Log::vOpenBySE(bool bStarting)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpAttachMap->Arg.m_bLog)
    return;
  
  if (gpLog)
    vFatal("Log::vOpenBySE() -- shared log already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Log* pLog = nullptr;
  
  if (bStarting) {
    pLog = Log::pAttachCreate();
    if (gpAttachMap->m_bVerbose)
      vTrace("Log %s : "
             "created new shared log (size %lldB, max %lld marks)\n\n"_Stz,
             C_LOG_REGION,
             sizeof(Log),
             C_LOG_ENTRIES);
    gpLog = new(pLog) Log;

  } else {
    pLog = Log::pAttachExisting();
    if (gpAttachMap->m_bVerbose)
      vTrace("Log %s : "
             "attached to existing shared log (size %lldB, max %lld marks)\n\n"_Stz,
             C_LOG_REGION,
             sizeof(Log),
             C_LOG_ENTRIES);
    
    gpLog = pLog;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s256 sWelcome;
  Engine::vGetWelcome(sWelcome);
  Log::vMarkInfo(sWelcome);
  
  Now oNow;
  Log::vMarkInfo("Time is: %s"_Stz,oNow.sz());

  s32 sMem;
  Magnitudes::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sClock;
  Magnitudes::vHzMag(Hardware::iClockFrequency(),sClock);
  
  Log::vMarkInfo("hw: %lld cores; %s memory; %s clock frequency"_Stz,
                     Hardware::iCoreCount(),
                     sMem.aStz(),
                     sClock.aStz()
                     );
  
  Arguments::vMark();
#endif
}

/*----------------------------------------------------------------------------*/
/* write Log to csv file                                                      */
/*----------------------------------------------------------------------------*/
void Log::vWriteCsv(bool bShortForm)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpLog)
    return;
  
  gpLog->vWriteToFile(bShortForm);
#endif
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write an log entry                                                         */
/*----------------------------------------------------------------------------*/
i8 Log::iAddMarkArg(i8 iEnumGroup, i8 iEnum, i8 iForeign,
                            Stz* aFormat, va_list xArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  char caMark[1024];
  
  vsnprintf(caMark,sizeof(caMark)-1,aFormat->aChar(),xArgs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  LogEntry* pLogEntry = pNewMark();
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  pLogEntry->vWrite(iEnumGroup,iEnum,iForeign,caMark);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* write an log entry                                                         */
/*----------------------------------------------------------------------------*/
i8 Log::iAddMarkVbl(i8 iEnumGroup, i8 iEnum, i8 iForeign,
                            Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  iAddMarkArg(iEnumGroup,iEnum,iForeign,aFormat,xArgs);
  va_end(xArgs);
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get a new log entry in Log                                                 */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewMark(void)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  LogEntry* pLogEntry = pNewMark_NL();
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* get a new log entry in Log (non-locking)                                   */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewMark_NL(void)
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
void Log::vFindRange_NL(i8& i_MinMark, i8& i_MaxMark)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  // should be a quicker way to do this
  
  i_MinMark = C_I8_MAX;
  i_MaxMark = C_I8_MIN;
  
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    
    if (-1 == m_oaEntry[i_Entry].m_i_Entry)
      return;
    
    if (m_oaEntry[i_Entry].m_i_Entry < i_MinMark)
      i_MinMark = m_oaEntry[i_Entry].m_i_Entry;
    
    if (m_oaEntry[i_Entry].m_i_Entry > i_MaxMark)
      i_MaxMark = m_oaEntry[i_Entry].m_i_Entry;
  }
}

/*----------------------------------------------------------------------------*/
/* calculate how full the shared log is                                       */
/*----------------------------------------------------------------------------*/
i8 Log::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  i8 nMarksInUse = 0;
  i8 iPercent = 0;
  
  i8 i_MinMark;
  i8 i_MaxMark;
  vFindRange_NL(i_MinMark,i_MaxMark);
  
  if (i_MinMark < i_MaxMark) {
    //
    // Example: 0 .. 25 __ [64]
    //          i_MinMark is 0
    //          i_Maxmark is 48
    //          m_nEntries  is 64
    // then only 0..25 is in use out of 64
    //
    nMarksInUse = (i_MaxMark - i_MinMark);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_MinMark is 48
    //          i_Maxmark is 25
    //          m_nEntries  is 64
    // then 48 .. 63 + 0 ..25 is in use
    //
    nMarksInUse = (m_nEntries - i_MinMark) + (i_MaxMark);
  }
  
  iPercent = (nMarksInUse * 100) / m_nEntries;
  
  return iPercent;
}

/*----------------------------------------------------------------------------*/
/* compute the size of Log given gpAttachMap->m_nLogEntrys                    */
/*----------------------------------------------------------------------------*/
i8 Log::iSize(void)
{
  i8 iLogSize = sizeof(Log);
  
  return iLogSize;
}

/*----------------------------------------------------------------------------*/
/* write Log to a csv file                                                    */
/*----------------------------------------------------------------------------*/
void Log::vWriteToFile(bool bShortForm)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  s256 sAmPath;
  sAmPath = "/tmp/Log.csv"_Stz;
  
  i8 iTotalDelta = 0;
  
  File* pActivityFile = new File();
  
  bool bOpened = pActivityFile->bOpenWrite(sAmPath);
  if (! bOpened) {
    vTrace("Unable to open activity file: %s\n"_Stz,sAmPath.aChar());
    return;
  }
  
  Now oNow;
  Log::vMarkInfo("Writing Log at: %s"_Stz,oNow.sz());
  
  s256 sWelcome;
  Engine::vGetWelcome(sWelcome);
  Log::vMarkInfo(sWelcome);
  
  i8 iUpTimeSsec = gpAttachMap->m_oBirth.iSecondsElapsed();
  Log::vMarkInfo("Uptime %llds Requests %lld"_Stz,
                         iUpTimeSsec,
                         C_UNKNOWN);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_MinMark;
  i8 i_MaxMark;
  
  vFindRange_NL(i_MinMark,i_MaxMark);
  
  LogEntry* pLogEntry = &m_oaEntry[i_MinMark % m_nEntries];
  pLogEntry->vDump(pActivityFile, bShortForm,
                       nullptr,iTotalDelta);
  
  for (i8 i_Entry = i_MinMark + 1; i_Entry <= i_MaxMark; i_Entry++) {
    
    pLogEntry = &m_oaEntry[i_Entry % m_nEntries];
    
    if (-1 == pLogEntry->m_i_Entry)
      break;
    
    pLogEntry->vDump(pActivityFile, bShortForm,
                         &m_oaEntry[(i_Entry - 1) % m_nEntries],
                         iTotalDelta);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pActivityFile->vClose();
  
  vTrace("Log written to: %s\n"_Stz,sAmPath.aChar());
  
  delete pActivityFile;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Log);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
