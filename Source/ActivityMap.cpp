/*============================================================================*/
/* ActivityMap.cpp - Activity Map                                             */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

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
/* dump ActivityMap                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDump(void)
{
  gpActivityMap->vDumpMap();
}

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActivityMap::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  RASSERT( ! gpActivityMap );
  
  bool bResult;
  if (0 == gpAttachMap->m_pGrid->m_iThisNode) {
    bResult = ActivityMap::bCreateNewShm();
    RASSERT(bResult);
  }
  else {
    bResult = ActivityMap::bOpenExistingShm();
    RASSERT(bResult);
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
  if (0 != gpAttachMap->m_pGrid->m_iThisNode)
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

  m_nMarks = 0;
  
  for (i8 i_Mark = 0; i_Mark < C_ACTIVITY_MARKS; i_Mark++) {
    ActivityMark* aActivityMark = &m_xActivity[i_Mark];
    aActivityMark->vInit();
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActivityMap::~ActivityMap(void)
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
  
  aActivityMark->vWrite(iJobId,aMark);
}

/*----------------------------------------------------------------------------*/
/* dump ActivityMap to a csv file in /tmp                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDumpMap(void)
{
  File* pActivityFile = new File();
  bool bOpened = pActivityFile->bOpen("/rapids/ActivityMap.csv"_Stz,C_OPEN_WRITE);
  RASSERT(bOpened);

  vLockMutex();
  
  for (i8 i_Mark = 0; i_Mark < m_nMarks; i_Mark++) {
    ActivityMark* aActivityMark = &m_xActivity[i_Mark];
    aActivityMark->vDump(pActivityFile);
  }
  
  vUnlockMutex();
  
  pActivityFile->vClose();
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
  
  ActivityMark* aActivityMark = &m_xActivity[m_nMarks];
  
  m_nMarks++;
  
  vUnlockMutex();

  return aActivityMark;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
