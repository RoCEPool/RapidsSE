/*============================================================================*/
/* ActivityMap.cpp - Activity Map                                             */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern Arguments  gxArguments;
extern AttachMap* gpAttachMap;

ActivityMap* gpActivityMap = nullptr;

/*----------------------------------------------------------------------------*/
/* -- Static -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close ActivityMap                                                          */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bCloseShm(void)
{
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a new ActivityMap shared memory segment                             */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bCreateNewShm(void)
{
  int iFD;
  SysCall xSysCall;
  s256 sRegionName;
  
  sRegionName = *C_ACTIVITY_REGION;
  
  // unlink previous  // open existing shm region
  bool bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( bResult ) {
    Shm::bUnlink(sRegionName,xSysCall);
  }
  
  // create a new shm region
  bResult  = Shm::bCreate(sRegionName,
                          sizeof(ActivityMap),
                          iFD,
                          false,
                          xSysCall);
  if ( ! bResult ) {
    vTrace("ActivityMap::bCreateNewShm: failed to create region %s\n"_Stz,
           sRegionName.aChar());
    NEVER_REACH;
  }

  // allocate memory for the shm region
  void* aAddr = Shm::aMap(iFD,
                          nullptr,
                          sizeof(ActivityMap),
                          true,
                          false,
                          xSysCall);
  if ( ! aAddr )  {
    vTrace("ActivityMap::bCreateNewShm: failed to map!\n"_Stz);
    Shm::bClose(iFD,xSysCall);
    NEVER_REACH;
  }

  gpActivityMap = static_cast<ActivityMap*>(aAddr);
  
  gpAttachMap->m_aActivityMap   = gpActivityMap;
  gpAttachMap->m_nActivityMarks = gxArguments.m_nActivityMarks;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open existing ActivityMap                                                  */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bOpenExistingShm(void)
{
  int iFD;
  SysCall xSysCall;
  s256 sRegionName;
  
  sRegionName = *C_ACTIVITY_REGION;
  
  // open existing shm region
  bool bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("ActivityMap::bOpenExistingShm: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return false;
  }

  // allocate memory for the shm region
  void* aAddr = Shm::aMap(iFD,
                          nullptr,
                          sizeof(ActivityMap),
                          true,
                          false,
                          xSysCall);
  if ( ! aAddr )  {
    vTrace("ActivityMap::bOpenExistingShm: failed to map!\n"_Stz);
    Shm::bClose(iFD,xSysCall);
    NEVER_REACH;
  }
  
  gpActivityMap = static_cast<ActivityMap*>(aAddr);
  
  return true;
}

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
/* open ActivityMap (called by API)                                           */
/*----------------------------------------------------------------------------*/
void ActivityMap::vOpenByApi(void)
{
  if (! gpAttachMap->m_aActivityMap)
    return;
  
  new ActivityMap;
  ActivityMap::vMark(0,"Opened by API"_Stz);
  
  Now xNow;
  ActivityMap::vMark(0,"Time is: "_Stz,xNow.sFormatted());
  
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
  
  Now xNow;
  ActivityMap::vMark(0,"Time is: %s"_Stz,xNow.sFormatted());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sMem;
  Magnitudes::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sClock;
  Magnitudes::vHzMag(Hardware::iClockFrequency(),sClock);
  
  ActivityMap::vMark(0,"hw: %lld cores; %s memory; %s clock frequency"_Stz,
                     Hardware::iCoreCount(),
                     sMem.aStr(),
                     sClock.aStr()
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
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActivityMap::operator new(size_t)
{
  RASSERT( ! gpActivityMap );
  
  bool bResult;
  if (0 == gpAttachMap->m_pGrid->m_iThisNode && gxArguments.m_bStart) {
    bResult = ActivityMap::bCreateNewShm();
    RASSERT(bResult);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityMap %s : created new map (size %lldB, max %lld marks)\n\n"_Stz,
             C_ACTIVITY_REGION,
             sizeof(ActivityMap),
             gpAttachMap->m_nActivityMarks);
  }
  else {
    bResult = ActivityMap::bOpenExistingShm();
    RASSERT(bResult);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActivityMap %s : opened existing map (size %lldB, max %lld marks)\n\n"_Stz,
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
    aActivity->bCloseShm();
  }
  else {
    aActivity->bCloseShm();
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActivityMap::ActivityMap(void)
{
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
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActivityMap::~ActivityMap(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/* -- Private ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write an ActivityMark                                                      */
/*----------------------------------------------------------------------------*/
void ActivityMap::vWriteMark(i8 iJobId, Stz* aFormat, va_list xArgs)
{
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
  i8 iTotalDelta = 0;
  
  File* pActivityFile = new File();
  
  s256 sAmPath = gxArguments.m_sAmPath;
  
  bool bOpened = pActivityFile->bOpen(sAmPath,C_OPEN_WRITE);
  
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
//  bool bOpened = pActivityFile->bOpen(sAmPath,C_OPEN_WRITE);
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
/* find dump range                                                            */
/*----------------------------------------------------------------------------*/
void ActivityMap::vFindRange(i8& i_MinMark, i8& i_MaxMark)
{
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
/* grab ActivityMap mutex                                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vLockMutex(void)
{
  i8 rCode = pthread_mutex_lock(&m_xMutex);
  if (0 != rCode)
    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* drop ActivityMap mutex                                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vUnlockMutex(void)
{
  i8 rCode = pthread_mutex_unlock(&m_xMutex);
  if (0 != rCode)
    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* get a new ActivityMap mark                                                 */
/*----------------------------------------------------------------------------*/
ActivityMark* ActivityMap::aNewActivityMark(void)
{
  vLockMutex();
  
  ActivityMark* aActivityMark = &m_xActivity[m_i_Mark % m_nMarks];
  
  aActivityMark->m_iMark = m_i_Mark;
  
  m_i_Mark++;
  
  vUnlockMutex();
  
  return aActivityMark;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
