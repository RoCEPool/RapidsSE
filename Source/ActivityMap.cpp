/*============================================================================*/
/* ActivityMap.cpp - Activity Map                                             */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

ActivityMap* gpActivityShm = nullptr;

/*----------------------------------------------------------------------------*/
/* -- Static -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close ActivityMap                                                          */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bClose(void)
{
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* create ActivityMap                                                         */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bCreate(void)
{
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* open existing ActivityMap                                                  */
/*----------------------------------------------------------------------------*/
bool ActivityMap::bOpen(void)
{
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* write an Activity mark                                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMark(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  gpActivityShm->vMarkShm(aFormat,xArgs);

  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* dump ActivityMap                                                           */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDump(void)
{
  gpActivityShm->vDumpShm();
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
  
  RASSERT( ! gpActivityShm );
  
  bool bResult;
  if (0 == gpAttachMap->m_pGrid->m_iThisNode) {
    bResult = ActivityMap::bCreate();
    RASSERT(bResult);
  }
  else {
    bResult = ActivityMap::bOpen();
    RASSERT(bResult);
  }
  
  return gpActivityShm;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActivityMap::operator delete(void* aAddress)
{
  ActivityMap* aActivity = static_cast<ActivityMap*>(aAddress);
  
  if (0 == gpAttachMap->m_pGrid->m_iThisNode) {
  }
  else {
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActivityMap::ActivityMap(void)
{
  
  // initialise mutex attribute
  MemLib::vZero(&m_xMutexAttr,sizeof(m_xMutexAttr),CN_SEMAPHORE);
  i8 iResult = pthread_mutexattr_init(&m_xMutexAttr);
  ASSERT( ! iResult);
  
  // set mutex attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xMutexAttr,PTHREAD_PROCESS_SHARED);
  ASSERT( ! iResult);
  
  // initialize the mutex
  MemLib::vZero(&m_xMutexAttr,sizeof(m_xMutex),CN_SEMAPHORE);
  iResult = pthread_mutex_init(&m_xMutex,&m_xMutexAttr);
  ASSERT( ! iResult);

  m_nMarks = 0;
  
  for (i8 i_Mark = 0; i_Mark < C_ACTIVITY_MARKS; i_Mark++) {
    ActivityMark* aActivityMark = &m_xActivity[i_Mark];
    //aActivityMark->vInit();
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
/* write an ActivityMap mark                                                     */
/*----------------------------------------------------------------------------*/
void ActivityMap::vMarkShm(Stz* aFormat, va_list xArgs)
{
  char aMark[1024];
  
  vsnprintf(aMark,sizeof(aMark)-1,aFormat->aChar(),xArgs);

  ActivityMark* aActivityMark = aNewActivityMark();
  
  aActivityMark->vWrite(aMark);
}

/*----------------------------------------------------------------------------*/
/* dump ActivityMap                                                              */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDumpShm(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* drop ActivityMap mutex                                                        */
/*----------------------------------------------------------------------------*/
void ActivityMap::vDropMutex(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* grab ActivityMap mutex                                                        */
/*----------------------------------------------------------------------------*/
void ActivityMap::vGrabMutex(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* get a new ActivityMap mark                                                    */
/*----------------------------------------------------------------------------*/
ActivityMark* ActivityMap::aNewActivityMark(void)
{
  vGrabMutex();
  
  ActivityMark* aActivityMark = &m_xActivity[m_nMarks];
  
  m_nMarks++;
  
  vDropMutex();

  return aActivityMark;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
