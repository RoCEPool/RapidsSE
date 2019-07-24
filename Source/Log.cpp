/*============================================================================*/
/* Log.cpp - Log                                                              */
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
/* create a new shm segment for Log                                           */
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
  
  // open a new shm region which will contain the activity map
  bResult = Shm::bOpenCreate(sRegionName,iRegionFd,oSysCall);
  if (! bResult)
    vFatal("Log::bOpenCreate: failed to create region %s\n"_Stz,
           sRegionName.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,sizeof(Log),oSysCall);
  if (! bResult)
    vFatal("Log::bOpenCreate: failed to extend region %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             sizeof(Log),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("Log::bOpenCreate: failed to map region!\n"_Stz);
  
  Log* pLog = static_cast<Log*>(aSegment);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm segment containing the Log                          */
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
    vFatal("Log::bOpenExisting: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             sizeof(Log),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)  {
    vTrace("Log::bOpenExisting: failed to map\n"_Stz);
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
/* add a log entry                                                            */
/*----------------------------------------------------------------------------*/
void Log::vNote(Stz* aFormat, ...)
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
/* add a log entry for api call                                               */
/*----------------------------------------------------------------------------*/
void Log::vNoteApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...)
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
void Log::vNoteApiEntry(i8 eApiCall, i8 iJobId)
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
void Log::vNoteApiError(i8 eApiCall, i8 iJobId, i8 iError)
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
void Log::vNoteApiExit(i8 eApiCall, i8 iJobId)
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
/* add a log entry for job                                                    */
/*----------------------------------------------------------------------------*/
void Log::vNoteJob(Jobs::Job* pJob, Stz* szFormat, ...)
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
/* add a log entry for resource                                               */
/*----------------------------------------------------------------------------*/
void Log::vNoteRes(eSR eSysRes,Stz* szFormat, ...)
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
/* add a section line log entry                                               */
/*----------------------------------------------------------------------------*/
void Log::vNoteSection(Stz* szSectionNm)
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
    
  vNote((Stz*)caSection);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a multi-line string entry                                              */
/*----------------------------------------------------------------------------*/
void Log::vNoteString(i8 iJobId, s48K& sString)
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
    LogEntry* pLogEntry = gpLog->pNewEntry_NL();
    pLogEntry->vSet(C_JOB_GROUP,iJobId,bForeign,sLine.aChar());
  }
  
  gpLog->vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void Log::vMonitorPortrait(s48K& sPortrait)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpLog) {
    sPortrait.vSprint("Log: activity map not configured\n"_Stz);
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
  if (! gpAttachMap->Arg.m_bActivityLog)
    return;
  
  if (gpLog)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Log* pLog = Log::pAttachExisting();
  if (gpAttachMap->m_bVerbose)
    vTrace("Log %s : "
           "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
           C_LOG_REGION,
           sizeof(Log),
           C_LOG_ENTRIES);
  
  gpLog = pLog;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Log::vNote("Opened by API"_Stz);
  
  Now oNow;
  Log::vNote("Time is: "_Stz,oNow.sz());
#endif
}

/*----------------------------------------------------------------------------*/
/* create or open Log (called by SE)                                          */
/*----------------------------------------------------------------------------*/
void Log::vOpenBySE(bool bStarting)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpAttachMap->Arg.m_bActivityLog)
    return;
  
  if (gpLog)
    vFatal("Log::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Log* pLog = nullptr;
  
  if (bStarting) {
    pLog = Log::pAttachCreate();
    if (gpAttachMap->m_bVerbose)
      vTrace("Log %s : "
             "created new map (size %lldB, max %lld marks)\n\n"_Stz,
             C_LOG_REGION,
             sizeof(Log),
             C_LOG_ENTRIES);
    gpLog = new(pLog) Log;

  } else {
    pLog = Log::pAttachExisting();
    if (gpAttachMap->m_bVerbose)
      vTrace("Log %s : "
             "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
             C_LOG_REGION,
             sizeof(Log),
             C_LOG_ENTRIES);
    
    gpLog = pLog;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s256 sWelcome;
  Engine::vGetWelcome(sWelcome);
  Log::vNote(sWelcome);
  
  Now oNow;
  Log::vNote("Time is: %s"_Stz,oNow.sz());

  s32 sMem;
  Magnitudes::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sClock;
  Magnitudes::vHzMag(Hardware::iClockFrequency(),sClock);
  
  Log::vNote("hw: %lld cores; %s memory; %s clock frequency"_Stz,
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
/* write an LogEntry                                                          */
/*----------------------------------------------------------------------------*/
i8 Log::iAddMarkArg(i8 iEnumGroup, i8 iEnum, i8 iForeign,
                            Stz* aFormat, va_list xArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  char caMark[1024];
  
  vsnprintf(caMark,sizeof(caMark)-1,aFormat->aChar(),xArgs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  LogEntry* pLogEntry = pNewEntry();
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  pLogEntry->vSet(iEnumGroup,iEnum,iForeign,caMark);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* write an LogEntry                                                          */
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
/* get a new mark in Log                                                      */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewEntry(void)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  LogEntry* pLogEntry = pNewEntry_NL();
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

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
    // then only 0..25 is in use out of 64
    //
    nEntriesInUse = (i_MaxEntry - i_MinEntry);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_MinEntry is 48
    //          i_MaxEntry is 25
    //          m_nEntries  is 64
    // then 48 .. 63 + 0 ..25 is in use
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
void Log::vWriteToFile(bool bShortForm)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  s256 sLogPath((Stz*)C_LOG_FILE);
  
  i8 iTotalDelta = 0;
  
  File* pLogFile = new File;
  
  bool bOpened = pLogFile->bOpenWrite(sLogPath);
  if (! bOpened) {
    vTrace("Unable to open log file: %s\n"_Stz,sLogPath.aChar());
    return;
  }
  
  Now oNow;
  Log::vNote("Writing Log at: %s"_Stz,oNow.sz());
  
  s256 sWelcome;
  Engine::vGetWelcome(sWelcome);
  Log::vNote(sWelcome);
  
  i8 iUpTimeSsec = gpAttachMap->m_oBirth.iSecondsElapsed();
  Log::vNote("Uptime %llds Requests %lld"_Stz,
                         iUpTimeSsec,
                         C_UNKNOWN);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_MinEntry;
  i8 i_MaxEntry;
  
  vFindRange_NL(i_MinEntry,i_MaxEntry);
  
  LogEntry* pLogEntry = &m_oaEntry[i_MinEntry % m_nEntries];
  pLogEntry->vDump(pLogFile, bShortForm,
                       nullptr,iTotalDelta);
  
  for (i8 i_Entry = i_MinEntry + 1; i_Entry <= i_MaxEntry; i_Entry++) {
    
    pLogEntry = &m_oaEntry[i_Entry % m_nEntries];
    
    if (-1 == pLogEntry->m_i_Entry)
      break;
    
    pLogEntry->vDump(pLogFile, bShortForm,
                         &m_oaEntry[(i_Entry - 1) % m_nEntries],
                         iTotalDelta);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pLogFile->vClose();
  
  vTrace("Log written to: %s\n"_Stz,sLogPath.aChar());
  
  delete pLogFile;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Log);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
