/*============================================================================*/
/* ActivityLog.cpp - Activity ShmLog                                             */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

ActivityLog* gpActivityLog = nullptr;


/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActivityLog::operator new(size_t, ActivityLog* pActivityLog)
{
  RASSERT(! gpActivityLog);
  
  return pActivityLog;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActivityLog::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActivityLog::ActivityLog(void)
: ShmLog(CN_ACTIVITYLOG)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
  
  if (0 != gpAttachMap->m_pGrid->m_iThisNode || Pthread::bIsJavaThread() )
    RASSERT(0);
  
  m_i_Mark = 0;
  m_nMarks = C_ACTIVITY_MARKS;
  
  for (i8 i_Mark = 0; i_Mark < m_nMarks; i_Mark++) {
    ActivityMark* pActivityMark = &m_oaMark[i_Mark];
    pActivityMark->vInit();
  }
  
  _checkInitialised_(CN_ACTIVITYLOG);
  
  _postcondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActivityLog::~ActivityLog(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* -- Private Static ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new ActivityLog shared memory segment                             */
/*----------------------------------------------------------------------------*/
ActivityLog* ActivityLog::pAttachCreate(void)
{
  int iRegionFd;
  SysCall oSysCall;
  s32 sRegionName;
  bool bResult;
  
  sRegionName = *C_ACTIVITY_REGION;
  
  // unlink the name of any previous segment
  IGNORE_RETURN( Shm::bUnlink(sRegionName,oSysCall) );
  
  // open a new shm region which will contain the activity map
  bResult = Shm::bOpenCreate(sRegionName,iRegionFd,oSysCall);
  if (! bResult)
    vFatal("ActivityLog::bOpenCreate: failed to create region %s\n"_Stz,
           sRegionName.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,sizeof(ActivityLog),oSysCall);
  if (! bResult)
    vFatal("ActivityLog::bOpenCreate: failed to extend region %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             sizeof(ActivityLog),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("ActivityLog::bOpenCreate: failed to map region!\n"_Stz);
  
  ActivityLog* pActivityLog = static_cast<ActivityLog*>(aSegment);
  
  return pActivityLog;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm region containing the ActivityLog                   */
/*----------------------------------------------------------------------------*/
ActivityLog* ActivityLog::pAttachExisting(void)
{
  int iFD;
  SysCall oSysCall;
  s32 sRegionName;
  
  sRegionName = *C_ACTIVITY_REGION;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if (! bResult)
    vFatal("ActivityLog::bOpenExisting: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             sizeof(ActivityLog),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)  {
    vTrace("ActivityLog::bOpenExisting: failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    NEVER_REACH;
  }
  
  ActivityLog* pActivityLog = static_cast<ActivityLog*>(aSegment);
  
  return pActivityLog;
}

/*----------------------------------------------------------------------------*/
/* -- Public Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api call                                        */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
       ! gpAttachMap->m_bMarkApiAll )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpActivityLog->iAddMarkArg(C_JOB_GROUP,iJobId,bForeign,aFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api entry                                       */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkApiEntry(i8 eApiCall, i8 iJobId)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpActivityLog->iAddMarkVbl(C_JOB_GROUP,iJobId,bForeign,">Api %s"_Stz,zApiName);
#endif
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api error                                       */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkApiError(i8 eApiCall, i8 iJobId, i8 iError)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpActivityLog->iAddMarkVbl(C_JOB_GROUP,iJobId,bForeign,"!Api %s Error %lld"_Stz,zApiName,iError);
#endif
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api exit                                        */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkApiExit(i8 eApiCall, i8 iJobId)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpActivityLog->iAddMarkVbl(C_JOB_GROUP,iJobId,bForeign,"<Api %s"_Stz,zApiName);
#endif
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark                                                     */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkInfo(Stz* aFormat, ...)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  gpActivityLog->iAddMarkArg(C_INF_GROUP,0,false,aFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkJob(Jobs::Job* pJob, Stz* szFormat, ...)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
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
  pJob->m_i_Activity = gpActivityLog->iAddMarkArg(C_JOB_GROUP,iJobId,bForeign,
                                                  szCombinedFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkRes(eSR eSysRes,Stz* szFormat, ...)
{

#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkResource[eSysRes] &&
       ! gpAttachMap->m_bMarkResourceAll )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  gpActivityLog->iAddMarkArg(_EG_SR,eSysRes,false,szFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkSection(Stz* szSectionNm)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
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
/* write a multi-line string                                                  */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMarkString(i8 iJobId, s48K& sString)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityLog )
    return;
  
  s4K sLine;
  char* aCharAdr = sString.aChar();

  bool bForeign = Job::bIsForeign(iJobId);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  gpActivityLog->vGrabLock();
  
  while (StrLib::bEachLine(&aCharAdr,sLine.aChar())) {
    //vTrace("Line: ]]%s[[\n"_Stz,sLine.aChar());
    ActivityMark* pActivityMark = gpActivityLog->pNewMark_NL();
    pActivityMark->vWrite(C_JOB_GROUP,iJobId,bForeign,sLine.aChar());
  }
  
  gpActivityLog->vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

#endif
}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void ActivityLog::vMonitorPortrait(s48K& sPortrait)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpActivityLog) {
    sPortrait.vSprint("ActivityLog: activity map not configured\n"_Stz);
    return;
  }
  
  gpActivityLog->vGrabLock();
  i8 iPercentFull = gpActivityLog->iPercentFull_NL();
  gpActivityLog->vDropLock();
  
  i8 iMapSize = gpActivityLog->iSize();
  s32 sShmMag;
  Magnitudes::vByteMag(iMapSize, sShmMag);
  
  s32 sArrayMag;
  Magnitudes::vUnitMag(gpActivityLog->m_nMarks, sArrayMag);

  sPortrait.vSprint("ActivityLog: shm %s array %s marks %lld; %lld%% full;\n"_Stz,
                    sShmMag.aStz(),
                    sArrayMag.aStz(),
                    gpActivityLog->m_i_Mark,
                    iPercentFull
                     );
#else
  sPortrait.vSprint("ActivityLog: not compiled WITH_ACTIVITY_MAP\n"_Stz);
#endif
}

/*----------------------------------------------------------------------------*/
/* open ActivityLog (called by API)                                           */
/*----------------------------------------------------------------------------*/
void ActivityLog::vOpenByApi(void)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpAttachMap->Arg.m_bActivityLog)
    return;
  
  if (gpActivityLog)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ActivityLog* pActivityLog = ActivityLog::pAttachExisting();
  if (gpAttachMap->m_bVerbose)
    vTrace("ActivityLog %s : "
           "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
           C_ACTIVITY_REGION,
           sizeof(ActivityLog),
           C_ACTIVITY_MARKS);
  
  gpActivityLog = pActivityLog;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ActivityLog::vMarkInfo("Opened by API"_Stz);
  
  Now oNow;
  ActivityLog::vMarkInfo("Time is: "_Stz,oNow.sz());
#endif
}

/*----------------------------------------------------------------------------*/
/* create or open ActivityLog (called by SE)                                  */
/*----------------------------------------------------------------------------*/
void ActivityLog::vOpenBySE(bool bStarting)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpAttachMap->Arg.m_bActivityLog)
    return;
  
  if (gpActivityLog)
    vFatal("ActivityLog::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ActivityLog* pActivityLog = nullptr;
  
  if (bStarting) {
    pActivityLog = ActivityLog::pAttachCreate();
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityLog %s : "
             "created new map (size %lldB, max %lld marks)\n\n"_Stz,
             C_ACTIVITY_REGION,
             sizeof(ActivityLog),
             C_ACTIVITY_MARKS);
    gpActivityLog = new(pActivityLog) ActivityLog;

  } else {
    pActivityLog = ActivityLog::pAttachExisting();
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityLog %s : "
             "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
             C_ACTIVITY_REGION,
             sizeof(ActivityLog),
             C_ACTIVITY_MARKS);
    
    gpActivityLog = pActivityLog;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s256 sWelcome;
  Engine::vGetWelcome(sWelcome);
  ActivityLog::vMarkInfo(sWelcome);
  
  Now oNow;
  ActivityLog::vMarkInfo("Time is: %s"_Stz,oNow.sz());

  s32 sMem;
  Magnitudes::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sClock;
  Magnitudes::vHzMag(Hardware::iClockFrequency(),sClock);
  
  ActivityLog::vMarkInfo("hw: %lld cores; %s memory; %s clock frequency"_Stz,
                     Hardware::iCoreCount(),
                     sMem.aStz(),
                     sClock.aStz()
                     );
  
  Arguments::vMark();
#endif
}

/*----------------------------------------------------------------------------*/
/* write ActivityLog to csv file                                              */
/*----------------------------------------------------------------------------*/
void ActivityLog::vWriteCsv(bool bShortForm)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpActivityLog)
    return;
  
  gpActivityLog->vWriteToFile(bShortForm);
#endif
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
i8 ActivityLog::iAddMarkArg(i8 iEnumGroup, i8 iEnum, i8 iForeign,
                            Stz* aFormat, va_list xArgs)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
  
  char caMark[1024];
  
  vsnprintf(caMark,sizeof(caMark)-1,aFormat->aChar(),xArgs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ActivityMark* pActivityMark = pNewMark();
  
  if (! pActivityMark)
    return C_UNKNOWN;
  
  pActivityMark->vWrite(iEnumGroup,iEnum,iForeign,caMark);
  
  return pActivityMark->m_iMark;
}

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
i8 ActivityLog::iAddMarkVbl(i8 iEnumGroup, i8 iEnum, i8 iForeign,
                            Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  iAddMarkArg(iEnumGroup,iEnum,iForeign,aFormat,xArgs);
  va_end(xArgs);
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get a new mark in ActivityLog                                              */
/*----------------------------------------------------------------------------*/
ActivityMark* ActivityLog::pNewMark(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  ActivityMark* pActivityMark = pNewMark_NL();
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

  return pActivityMark;
}

/*----------------------------------------------------------------------------*/
/* get a new mark in ActivityLog (non-locking)                                */
/*----------------------------------------------------------------------------*/
ActivityMark* ActivityLog::pNewMark_NL(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);

  m_okLock.vAssertWrite();
  
  ActivityMark* pActivityMark = &m_oaMark[m_i_Mark % m_nMarks];
  
  pActivityMark->m_iMark = m_i_Mark;
  
  m_i_Mark++;
  
  return pActivityMark;
}

/*----------------------------------------------------------------------------*/
/* find active range                                                          */
/*----------------------------------------------------------------------------*/
void ActivityLog::vFindRange_NL(i8& i_MinMark, i8& i_MaxMark)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  // should be a quicker way to do this
  
  i_MinMark = C_I8_MAX;
  i_MaxMark = C_I8_MIN;
  
  for (i8 i_Mark = 0; i_Mark < m_nMarks; i_Mark++) {
    
    if (-1 == m_oaMark[i_Mark].m_iMark)
      return;
    
    if (m_oaMark[i_Mark].m_iMark < i_MinMark)
      i_MinMark = m_oaMark[i_Mark].m_iMark;
    
    if (m_oaMark[i_Mark].m_iMark > i_MaxMark)
      i_MaxMark = m_oaMark[i_Mark].m_iMark;
  }
}

/*----------------------------------------------------------------------------*/
/* calculate how full the activity map is                                     */
/*----------------------------------------------------------------------------*/
i8 ActivityLog::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
  
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
    //          m_nMarks  is 64
    // then only 0..25 is in use out of 64
    //
    nMarksInUse = (i_MaxMark - i_MinMark);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_MinMark is 48
    //          i_Maxmark is 25
    //          m_nMarks  is 64
    // then 48 .. 63 + 0 ..25 is in use
    //
    nMarksInUse = (m_nMarks - i_MinMark) + (i_MaxMark);
  }
  
  iPercent = (nMarksInUse * 100) / m_nMarks;
  
  return iPercent;
}

/*----------------------------------------------------------------------------*/
/* compute the size of ActivityLog given gpAttachMap->m_nActivityMarks        */
/*----------------------------------------------------------------------------*/
i8 ActivityLog::iSize(void)
{
  i8 iMapSize = sizeof(ActivityLog);
  
  return iMapSize;
}

/*----------------------------------------------------------------------------*/
/* write ActivityLog to a csv file                                            */
/*----------------------------------------------------------------------------*/
void ActivityLog::vWriteToFile(bool bShortForm)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG,_FOOTER_);
  
  s256 sAmPath;
  sAmPath = "/tmp/ActivityLog.csv"_Stz;
  
  i8 iTotalDelta = 0;
  
  File* pActivityFile = new File();
  
  bool bOpened = pActivityFile->bOpenWrite(sAmPath);
  if (! bOpened) {
    vTrace("Unable to open activity file: %s\n"_Stz,sAmPath.aChar());
    return;
  }
  
  Now oNow;
  ActivityLog::vMarkInfo("Writing ActivityLog at: %s"_Stz,oNow.sz());
  
  s256 sWelcome;
  Engine::vGetWelcome(sWelcome);
  ActivityLog::vMarkInfo(sWelcome);
  
  i8 iUpTimeSsec = gpAttachMap->m_oBirth.iSecondsElapsed();
  ActivityLog::vMarkInfo("Uptime %llds Requests %lld"_Stz,
                         iUpTimeSsec,
                         C_UNKNOWN);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_MinMark;
  i8 i_MaxMark;
  
  vFindRange_NL(i_MinMark,i_MaxMark);
  
  ActivityMark* pActivityMark = &m_oaMark[i_MinMark % m_nMarks];
  pActivityMark->vDump(pActivityFile, bShortForm,
                       nullptr,iTotalDelta);
  
  for (i8 i_Mark = i_MinMark + 1; i_Mark <= i_MaxMark; i_Mark++) {
    
    pActivityMark = &m_oaMark[i_Mark % m_nMarks];
    
    if (-1 == pActivityMark->m_iMark)
      break;
    
    pActivityMark->vDump(pActivityFile, bShortForm,
                         &m_oaMark[(i_Mark - 1) % m_nMarks],
                         iTotalDelta);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pActivityFile->vClose();
  
  vTrace("ActivityLog written to: %s\n"_Stz,sAmPath.aChar());
  
  delete pActivityFile;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ActivityLog::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYLOG);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
