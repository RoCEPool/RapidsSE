/*============================================================================*/
/* Activity.cpp - Activity                                                    */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

Activity* gpActivityShm = nullptr;

/*----------------------------------------------------------------------------*/
/* -- Static -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close activity                                                             */
/*----------------------------------------------------------------------------*/
bool Activity::bClose(void)
{
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* create activity                                                            */
/*----------------------------------------------------------------------------*/
bool Activity::bCreate(void)
{
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* open existing activity                                                     */
/*----------------------------------------------------------------------------*/
bool Activity::bOpen(void)
{
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* write an activity mark                                                     */
/*----------------------------------------------------------------------------*/
void Activity::vMark(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  gpActivityShm->vMarkShm(aFormat,xArgs);

  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* dump activity                                                              */
/*----------------------------------------------------------------------------*/
void Activity::vDump(void)
{
  gpActivityShm->vDumpShm();
}

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Activity::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  RASSERT( ! gpActivityShm );
  
  bool bResult;
  if (0 == gpAttachMap->m_pGrid->m_iThisNode) {
    bResult = Activity::bCreate();
    RASSERT(bResult);
  }
  else {
    bResult = Activity::bOpen();
    RASSERT(bResult);
  }
  
  return gpActivityShm;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Activity::operator delete(void* aAddress)
{
  Activity* aActivity = static_cast<Activity*>(aAddress);
  
  if (0 == gpAttachMap->m_pGrid->m_iThisNode) {
  }
  else {
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Activity::Activity(void)
{
  
  // initialise mutex attribute
  MemLib::vZero(&m_xActivityMutexAttr,sizeof(m_xActivityMutexAttr),CN_SEMAPHORE);
  i8 iResult = pthread_mutexattr_init(&m_xActivityMutexAttr);
  ASSERT( ! iResult);
  
  // set mutex attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xActivityMutexAttr,PTHREAD_PROCESS_SHARED);
  ASSERT( ! iResult);
  
  // initialize the mutex
  MemLib::vZero(&m_xActivityMutexAttr,sizeof(m_xActivityMutex),CN_SEMAPHORE);
  iResult = pthread_mutex_init(&m_xActivityMutex,&m_xActivityMutexAttr);
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
Activity::~Activity(void)
{
}


/*----------------------------------------------------------------------------*/
/* -- Private ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write an activity mark                                                     */
/*----------------------------------------------------------------------------*/
void Activity::vMarkShm(Stz* aFormat, va_list xArgs)
{
  char aMark[1024];
  
  vsnprintf(aMark,sizeof(aMark)-1,aFormat->aChar(),xArgs);

  ActivityMark* aActivityMark = aNewActivityMark();
  
  aActivityMark->vWrite(aMark);
}

/*----------------------------------------------------------------------------*/
/* dump activity                                                              */
/*----------------------------------------------------------------------------*/
void Activity::vDumpShm(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* drop activity mutex                                                        */
/*----------------------------------------------------------------------------*/
void Activity::vDropMutex(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* grab activity mutex                                                        */
/*----------------------------------------------------------------------------*/
void Activity::vGrabMutex(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* get a new activity mark                                                    */
/*----------------------------------------------------------------------------*/
ActivityMark* Activity::aNewActivityMark(void)
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
