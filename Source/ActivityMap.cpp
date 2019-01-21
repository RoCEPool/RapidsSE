/*============================================================================*/
/* ActivityMap.cpp - Activity Map                                             */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern Arguments  gxArguments;
extern AttachMap* gpAttachMap;

ActivityMap* gpActivityMap = nullptr;


/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActivityMap::operator new(size_t)
{
  RASSERT(! gpActivityMap);
  
  bool bResult;
  if (0 == gpAttachMap->m_pGrid->m_iThisNode && gxArguments.m_bStart) {
    bResult = ActivityMap::bCreateNew();
    RASSERT(bResult);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityMap %s : "
             "created new map (size %lldB, max %lld marks)\n\n"_Stz,
             C_ACTIVITY_REGION,
             sizeof(ActivityMap),
             gpAttachMap->m_nActivityMarks);
  }
  else {
    bResult = ActivityMap::bAttachExisting();
    RASSERT(bResult);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityMap %s : "
             "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
             C_ACTIVITY_REGION,
             sizeof(ActivityMap),
             gpAttachMap->m_nActivityMarks);
  }
  
  return gpActivityMap;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActivityMap::operator delete(void* aAddress)
{
  ActivityMap* aActivity = static_cast<ActivityMap*>(aAddress);
  
  if (0 == gpAttachMap->m_pGrid->m_iThisNode) {
    //aActivity->bCloseShm();
  }
  else {
    //aActivity->bCloseShm();
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActivityMap::ActivityMap(void)
: Object(CN_ACTIVITYMAP)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  if (0 != gpAttachMap->m_pGrid->m_iThisNode || PosixThread::bIsJavaThread() )
    return;
  
  // initialise mutex attribute
  MemLib::vZero(&m_xMutexAttr,sizeof(m_xMutexAttr),CN_SEMAPHORE);
  i8 iResult = pthread_mutexattr_init(&m_xMutexAttr);
  ASSERT( ! iResult);
  
  // set mutex attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xMutexAttr,PTHREAD_PROCESS_SHARED);
  ASSERT( ! iResult);
  
  // initialize the mutex
  MemLib::vZero(&m_xMutex,sizeof(m_xMutex),CN_SEMAPHORE);
  iResult = pthread_mutex_init(&m_xMutex,&m_xMutexAttr);
  ASSERT( ! iResult);
  
  m_i_Mark = 0;
  m_nMarks = gxArguments.m_nActivityMarks;
  
  for (i8 i_Mark = 0; i_Mark < m_nMarks; i_Mark++) {
    ActivityMark* aActivityMark = &m_xActivity[i_Mark];
    aActivityMark->vInit();
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
/* attach to existing shm region containing the ActivityMap                   */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bAttachExisting(void)
{
  int iFD;
  SysCall oSysCall;
  s32 sRegionName;
  
  sRegionName = *C_ACTIVITY_REGION;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if (! bResult) {
    vTrace("ActivityMap::bAttachExisting: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return false;
  }

  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             sizeof(ActivityMap),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)  {
    vTrace("ActivityMap::bAttachExisting: failed to map!\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    NEVER_REACH;
  }
  
  gpActivityMap = static_cast<ActivityMap*>(aSegment);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a new ActivityMap shared memory segment                             */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bCreateNew(void)
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
    vFatal("ActivityMap::bCreateNewShm: failed to create region %s\n"_Stz,
           sRegionName.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,sizeof(ActivityMap),oSysCall);
  if (! bResult)
    vFatal("ActivityMap::bCreateNewShm: failed to extend region %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             sizeof(ActivityMap),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("ActivityMap::bCreateNewShm: failed to map region!\n"_Stz);
  
  gpActivityMap = static_cast<ActivityMap*>(aSegment);
  
  gpAttachMap->m_aActivityMap   = gpActivityMap;
  gpAttachMap->m_nActivityMarks = gxArguments.m_nActivityMarks;
  
  return true;
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
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMonitorPortrait(s48K& sPortrait)
{
  if (! gpAttachMap->m_aActivityMap) {
    sPortrait.vSprint("ActivityMap: no activity map\n"_Stz);
    return;
  }
  
  gpAttachMap->m_aActivityMap->vLockMutex();
  i8 iPercentFull = gpAttachMap->m_aActivityMap->iPercentFull_NL();
  gpAttachMap->m_aActivityMap->vUnlockMutex();
  
  i8 iMapSize = iSize();
  s32 sShmMag;
  Magnitudes::vByteMag(iMapSize, sShmMag);
  
  s32 sArrayMag;
  Magnitudes::vUnitMag(gpAttachMap->m_aActivityMap->m_nMarks, sArrayMag);

  sPortrait.vSprint("ActivityMap: shm %s array %s marks %lld; %lld%% full;\n"_Stz,
                    sShmMag.aStz(),
                    sArrayMag.aStz(),
                    gpAttachMap->m_aActivityMap->m_i_Mark,
                    iPercentFull
                     );
}

/*----------------------------------------------------------------------------*/
/* open ActivityMap (called by API)                                           */
/*----------------------------------------------------------------------------*/
void ActivityMap::vOpenByApi(void)
{
  if (! gpAttachMap->m_aActivityMap)
    return;
  
  new ActivityMap;
  ActivityMap::vMark(0,"Opened by API"_Stz);
  
  Now oNow;
  ActivityMap::vMark(0,"Time is: "_Stz,oNow.sFormatted());
  
}

/*----------------------------------------------------------------------------*/
/* create or open ActivityMap (called by SE)                                  */
/*----------------------------------------------------------------------------*/
void ActivityMap::vOpenBySE(void)
{
  if (! gxArguments.m_bActivityMap)
    return;
  
  new ActivityMap;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ActivityMap::vMark(0,gxArguments.m_sWelcome);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Now oNow;
  ActivityMap::vMark(0,"Time is: %s"_Stz,oNow.sFormatted());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sMem;
  Magnitudes::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sClock;
  Magnitudes::vHzMag(Hardware::iClockFrequency(),sClock);
  
  ActivityMap::vMark(0,"hw: %lld cores; %s memory; %s clock frequency"_Stz,
                     Hardware::iCoreCount(),
                     sMem.aStz(),
                     sClock.aStz()
                     );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Arguments::vMark();
}

/*----------------------------------------------------------------------------*/
/* dump ActivityMap                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDump(void)
{
  if (gpActivityMap)
    gpActivityMap->vDumpMap();
}

/*----------------------------------------------------------------------------*/
/* compute the size of ActivityMap given gpAttachMap->m_nActivityMarks        */
/*----------------------------------------------------------------------------*/
i8 ActivityMap::iSize(void)
{
  i8 iMapSize = sizeof(ActivityMap)
                  - sizeof(ActivityMark)
                  + sizeof(ActivityMark) * gpAttachMap->m_nActivityMarks;
  
  return iMapSize;
}


/*----------------------------------------------------------------------------*/
/* -- Public -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
void ActivityMap::vWriteMark(i8 iJobId, Stz* aFormat, va_list xArgs)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  char aMark[1024];
  
  vsnprintf(aMark,sizeof(aMark)-1,aFormat->aChar(),xArgs);

  ActivityMark* aActivityMark = aNewActivityMark();
  
  if (! aActivityMark)
    return;
  
  aActivityMark->vWrite(iJobId,aMark);
}

/*----------------------------------------------------------------------------*/
/* dump ActivityMap to a csv file in /tmp                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDumpMap(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  i8 iTotalDelta = 0;
  
  File* pActivityFile = new File();
  
  s256 sAmPath = gxArguments.m_sAmPath;
  
  bool bOpened = pActivityFile->bOpenWrite(sAmPath);
  
  if (! bOpened) {
    vTrace("Unable to open activity file: %s\n"_Stz,sAmPath.aChar());
    return;
  }

  vLockMutex();
  
  i8 i_MinMark;
  i8 i_MaxMark;
  
  vFindRange(i_MinMark,i_MaxMark);
  
  ActivityMark* aActivityMark = &m_xActivity[i_MinMark % m_nMarks];
  aActivityMark->vDump(pActivityFile,
                       0,nullptr,iTotalDelta);
  
  for (i8 i_Mark = i_MinMark + 1; i_Mark <= i_MaxMark; i_Mark++) {
    
    aActivityMark = &m_xActivity[i_Mark % m_nMarks];
    
    if (-1 == aActivityMark->m_iMark)
      break;
    
    aActivityMark->vDump(pActivityFile,
                         i_Mark,
                         &m_xActivity[(i_Mark - 1) % m_nMarks],
                         iTotalDelta);
  }
  
  vUnlockMutex();
  
  pActivityFile->vClose();
  
  vTrace("ActivityMap dumped : %s\n"_Stz,sAmPath.aChar());
  
  delete pActivityFile;
}
/*----------------------------------------------------------------------------*/
//void ActivityMap::vDumpMap(void)
//{
//  i8 iTotalDelta = 0;
//
//  File* pActivityFile = new File();
//
//  s256 sAmPath = gxArguments.m_sAmPath;
//
//  bool bOpened = pActivityFile->bOpenWrite(sAmPath);
//
//  if (! bOpened) {
//    vTrace("Unable to open activity file: %s\n"_Stz,sAmPath.aChar());
//    return;
//  }
//
//  vLockMutex();
//
//  ActivityMark* aActivityMark = &m_xActivity[0];
//  aActivityMark->vDump(pActivityFile,
//                       0,nullptr,iTotalDelta);
//
//  for (i8 i_Mark = 1; i_Mark < m_nMarks; i_Mark++) {
//    aActivityMark = &m_xActivity[i_Mark];
//    aActivityMark->vDump(pActivityFile,
//                         i_Mark,&m_xActivity[i_Mark - 1],iTotalDelta);
//  }
//
//  vUnlockMutex();
//
//  pActivityFile->vClose();
//
//  vTrace("ActivityMap dumped : %s\n"_Stz,sAmPath.aChar());
//}

/*----------------------------------------------------------------------------*/
/* find active range                                                          */
/*----------------------------------------------------------------------------*/
void ActivityMap::vFindRange(i8& i_MinMark, i8& i_MaxMark)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  // should be a quicker way to do this
  
  i_MinMark = C_I8_MAX;
  i_MaxMark = C_I8_MIN;
  
  for (i8 i_Mark = 0; i_Mark < m_nMarks; i_Mark++) {
    
    if (-1 == m_xActivity[i_Mark].m_iMark)
      return;
          
    if (m_xActivity[i_Mark].m_iMark < i_MinMark)
      i_MinMark = m_xActivity[i_Mark].m_iMark;
    
    if (m_xActivity[i_Mark].m_iMark > i_MaxMark)
      i_MaxMark = m_xActivity[i_Mark].m_iMark;
  }
}

/*----------------------------------------------------------------------------*/
/* calculate how full the activity map is                                     */
/*----------------------------------------------------------------------------*/
i8 ActivityMap::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  i8 nMarksInUse = 0;
  i8 iPercent = 0;
  
  i8 i_MinMark;
  i8 i_MaxMark;
  vFindRange(i_MinMark,i_MaxMark);
  
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
/* grab ActivityMap mutex                                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vLockMutex(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  i8 rCode = pthread_mutex_lock(&m_xMutex);
  if (0 != rCode)
    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* drop ActivityMap mutex                                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vUnlockMutex(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  if (0 != rCode)
    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* get a new ActivityMap mark                                                 */
/*----------------------------------------------------------------------------*/
ActivityMark* ActivityMap::aNewActivityMark(void)
{
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);

  vLockMutex();
  
  ActivityMark* aActivityMark = &m_xActivity[m_i_Mark % m_nMarks];
  
  aActivityMark->m_iMark = m_i_Mark;
  
  m_i_Mark++;
  
  vUnlockMutex();
  
  return aActivityMark;
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
