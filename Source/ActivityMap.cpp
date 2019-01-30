/*============================================================================*/
/* ActivityMap.cpp - Activity Map                                             */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

ActivityMap* gpActivityMap = nullptr;


/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActivityMap::operator new(size_t, ActivityMap* pActivityMap)
{
  RASSERT(! gpActivityMap);
  
  return pActivityMap;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActivityMap::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActivityMap::ActivityMap(void)
: Object(CN_ACTIVITYMAP)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
  if (0 != gpAttachMap->m_pGrid->m_iThisNode || Pthread::bIsJavaThread() )
    RASSERT(0);
  
  m_okLock.vInitLock(LK_ACTIVITY_MAP);
  
  m_i_Mark = 0;
  m_nMarks = C_ACTIVITY_MARKS;
  
  for (i8 i_Mark = 0; i_Mark < m_nMarks; i_Mark++) {
    ActivityMark* pActivityMark = &m_oaMark[i_Mark];
    pActivityMark->vInit();
  }
  
  _checkInitialised_(CN_ACTIVITYMAP);
  
  _postcondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActivityMap::~ActivityMap(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* -- Private Static ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new ActivityMap shared memory segment                             */
/*----------------------------------------------------------------------------*/
ActivityMap* ActivityMap::pAttachCreate(void)
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
    vFatal("ActivityMap::bOpenCreate: failed to create region %s\n"_Stz,
           sRegionName.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,sizeof(ActivityMap),oSysCall);
  if (! bResult)
    vFatal("ActivityMap::bOpenCreate: failed to extend region %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             sizeof(ActivityMap),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("ActivityMap::bOpenCreate: failed to map region!\n"_Stz);
  
  ActivityMap* pActivityMap = static_cast<ActivityMap*>(aSegment);
  
  return pActivityMap;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm region containing the ActivityMap                   */
/*----------------------------------------------------------------------------*/
ActivityMap* ActivityMap::pAttachExisting(void)
{
  int iFD;
  SysCall oSysCall;
  s32 sRegionName;
  
  sRegionName = *C_ACTIVITY_REGION;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if (! bResult)
    vFatal("ActivityMap::bOpenExisting: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             sizeof(ActivityMap),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)  {
    vTrace("ActivityMap::bOpenExisting: failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    NEVER_REACH;
  }
  
  ActivityMap* pActivityMap = static_cast<ActivityMap*>(aSegment);
  
  return pActivityMap;
}

/*----------------------------------------------------------------------------*/
/* -- Public Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write an Activity mark                                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMark(i8 iJobId, Stz* aFormat, ...)
{
  if ( ! gpActivityMap )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  gpActivityMap->vWriteMark(iJobId,aFormat,xArgs);

  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api call                                        */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMarkApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...)
{
  if ( ! gpActivityMap )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
       ! gpAttachMap->m_bMarkApiAll )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  gpActivityMap->vWriteMark(iJobId,aFormat,xArgs);
  
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api entry                                       */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMarkApiEntry(i8 eApiCall, i8 iJobId)
{
  if ( ! gpActivityMap )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  gpActivityMap->vWriteMark(iJobId,">Api %s"_Stz,zApiName);
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api error                                       */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMarkApiError(i8 eApiCall, i8 iJobId, i8 iError)
{
  if ( ! gpActivityMap )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  gpActivityMap->vWriteMark(iJobId,"!Api %s Error %lld"_Stz,zApiName,iError);
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark for api exit                                        */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMarkApiExit(i8 eApiCall, i8 iJobId)
{
  if ( ! gpActivityMap )
    return;
  
  if ( ! gpAttachMap->m_bMarkApi[eApiCall] &&
      ! gpAttachMap->m_bMarkApiAll )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  gpActivityMap->vWriteMark(iJobId,"<Api %s"_Stz,zApiName);
}

/*----------------------------------------------------------------------------*/
/* write a multi-line string                                                  */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMarkString(i8 iJobId, s48K& sString)
{
  if ( ! gpActivityMap )
    return;
  
  s4K sLine;
  char* aCharAdr = sString.aChar();

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  gpActivityMap->vGrabLock();
  
  while (StrLib::bEachLine(&aCharAdr,sLine.aChar())) {
    //vTrace("Line: ]]%s[[\n"_Stz,sLine.aChar());
    ActivityMark* pActivityMark = gpActivityMap->pNewMark_NL();
    pActivityMark->vWrite(iJobId,sLine.aChar());
  }
  
  gpActivityMap->vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMonitorPortrait(s48K& sPortrait)
{
  if (! gpActivityMap) {
    sPortrait.vSprint("ActivityMap: no activity map\n"_Stz);
    return;
  }
  
  gpActivityMap->vGrabLock();
  i8 iPercentFull = gpActivityMap->iPercentFull_NL();
  gpActivityMap->vDropLock();
  
  i8 iMapSize = gpActivityMap->iSize();
  s32 sShmMag;
  Magnitudes::vByteMag(iMapSize, sShmMag);
  
  s32 sArrayMag;
  Magnitudes::vUnitMag(gpActivityMap->m_nMarks, sArrayMag);

  sPortrait.vSprint("ActivityMap: shm %s array %s marks %lld; %lld%% full;\n"_Stz,
                    sShmMag.aStz(),
                    sArrayMag.aStz(),
                    gpActivityMap->m_i_Mark,
                    iPercentFull
                     );
}

/*----------------------------------------------------------------------------*/
/* open ActivityMap (called by API)                                           */
/*----------------------------------------------------------------------------*/
void ActivityMap::vOpenByApi(void)
{
  if (! gpAttachMap->Arg.m_bActivityMap)
    return;
  
  if (gpActivityMap)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ActivityMap* pActivityMap = ActivityMap::pAttachExisting();
  if (gpAttachMap->m_bVerbose)
    vTrace("ActivityMap %s : "
           "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
           C_ACTIVITY_REGION,
           sizeof(ActivityMap),
           C_ACTIVITY_MARKS);
  
  gpActivityMap = pActivityMap;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ActivityMap::vMark(0,"Opened by API"_Stz);
  
  Now oNow;
  ActivityMap::vMark(0,"Time is: "_Stz,oNow.sFormatted());
}

/*----------------------------------------------------------------------------*/
/* create or open ActivityMap (called by SE)                                  */
/*----------------------------------------------------------------------------*/
void ActivityMap::vOpenBySE(bool bStarting)
{
  if (! gpAttachMap->Arg.m_bActivityMap)
    return;
  
  if (gpActivityMap)
    vFatal("ActivityMap::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ActivityMap* pActivityMap = nullptr;
  
  if (bStarting) {
    pActivityMap = ActivityMap::pAttachCreate();
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityMap %s : "
             "created new map (size %lldB, max %lld marks)\n\n"_Stz,
             C_ACTIVITY_REGION,
             sizeof(ActivityMap),
             C_ACTIVITY_MARKS);
    gpActivityMap = new(pActivityMap) ActivityMap;

  } else {
    pActivityMap = ActivityMap::pAttachExisting();
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityMap %s : "
             "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
             C_ACTIVITY_REGION,
             sizeof(ActivityMap),
             C_ACTIVITY_MARKS);
    
    gpActivityMap = pActivityMap;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s256 sWelcome;
  Engine::vGetWelcome(sWelcome);
  ActivityMap::vMark(0,sWelcome);
  
  Now oNow;
  ActivityMap::vMark(0,"Time is: %s"_Stz,oNow.sFormatted());

  s32 sMem;
  Magnitudes::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sClock;
  Magnitudes::vHzMag(Hardware::iClockFrequency(),sClock);
  
  ActivityMap::vMark(0,"hw: %lld cores; %s memory; %s clock frequency"_Stz,
                     Hardware::iCoreCount(),
                     sMem.aStz(),
                     sClock.aStz()
                     );
  
  Arguments::vMark();
}

/*----------------------------------------------------------------------------*/
/* write ActivityMap to csv file                                              */
/*----------------------------------------------------------------------------*/
void ActivityMap::vWriteCsv()
{
  if (! gpActivityMap)
    return;
  
  gpActivityMap->vWriteToFile();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a new mark in ActivityMap                                              */
/*----------------------------------------------------------------------------*/
ActivityMark* ActivityMap::pNewMark(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  ActivityMark* pActivityMark = pNewMark_NL();
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

  return pActivityMark;
}

/*----------------------------------------------------------------------------*/
/* get a new mark in ActivityMap (non-locking)                                */
/*----------------------------------------------------------------------------*/
ActivityMark* ActivityMap::pNewMark_NL(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  m_okLock.vAssertWrite();
  
  ActivityMark* pActivityMark = &m_oaMark[m_i_Mark % m_nMarks];
  
  pActivityMark->m_iMark = m_i_Mark;
  
  m_i_Mark++;
  
  return pActivityMark;
}

/*----------------------------------------------------------------------------*/
/* drop lock on ActivityMap                                                   */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDropLock(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find active range                                                          */
/*----------------------------------------------------------------------------*/
void ActivityMap::vFindRange_NL(i8& i_MinMark, i8& i_MaxMark)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
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
/* grab lock on ActivityMap                                                   */
/*----------------------------------------------------------------------------*/
void ActivityMap::vGrabLock(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* calculate how full the activity map is                                     */
/*----------------------------------------------------------------------------*/
i8 ActivityMap::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
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
/* compute the size of ActivityMap given gpAttachMap->m_nActivityMarks        */
/*----------------------------------------------------------------------------*/
i8 ActivityMap::iSize(void)
{
  i8 iMapSize = sizeof(ActivityMap)
  - sizeof(ActivityMark)
  + sizeof(ActivityMark) * C_ACTIVITY_MARKS;
  
  return iMapSize;
}

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
void ActivityMap::vWriteMark(i8 iJobId, Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vWriteMark(iJobId,aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
void ActivityMap::vWriteMark(i8 iJobId, Stz* aFormat, va_list xArgs)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
  char caMark[1024];
  
  vsnprintf(caMark,sizeof(caMark)-1,aFormat->aChar(),xArgs);
  
  ActivityMark* pActivityMark = pNewMark();
  
  if (! pActivityMark)
    return;
  
  pActivityMark->vWrite(iJobId,caMark);
}

/*----------------------------------------------------------------------------*/
/* write ActivityMap to a csv file                                            */
/*----------------------------------------------------------------------------*/
void ActivityMap::vWriteToFile(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
  s256 sAmPath;
  sAmPath = "/tmp/ActivityMap.csv"_Stz;
  
  i8 iTotalDelta = 0;
  
  File* pActivityFile = new File();
  
  bool bOpened = pActivityFile->bOpenWrite(sAmPath);
  if (! bOpened) {
    vTrace("Unable to open activity file: %s\n"_Stz,sAmPath.aChar());
    return;
  }
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_MinMark;
  i8 i_MaxMark;
  
  vFindRange_NL(i_MinMark,i_MaxMark);
  
  ActivityMark* pActivityMark = &m_oaMark[i_MinMark % m_nMarks];
  pActivityMark->vDump(pActivityFile,
                       0,nullptr,iTotalDelta);
  
  for (i8 i_Mark = i_MinMark + 1; i_Mark <= i_MaxMark; i_Mark++) {
    
    pActivityMark = &m_oaMark[i_Mark % m_nMarks];
    
    if (-1 == pActivityMark->m_iMark)
      break;
    
    pActivityMark->vDump(pActivityFile,
                         i_Mark,
                         &m_oaMark[(i_Mark - 1) % m_nMarks],
                         iTotalDelta);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pActivityFile->vClose();
  
  vTrace("ActivityMap written to: %s\n"_Stz,sAmPath.aChar());
  
  delete pActivityFile;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ActivityMap::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
