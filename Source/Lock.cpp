/*============================================================================*/
/* Lock.cpp -- Shared Read / Exclusive Write Lock                             */
/*============================================================================*/

#include "StorageEngine.h"
#include "System.h"
#include "Pool.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Lock* System::_pLock(void* aAddr)
{
  Lock* pLock = reinterpret_cast<Lock*>(aAddr);
  
  RASSERT_TYPE(pLock,CN_LOCK);
  
  return pLock;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Lock::operator new(size_t, Lock* pLock)
{
  return pLock;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Lock::operator delete(void*)
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Lock::Lock(void)
: Object(CN_LOCK)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  m_uLockWord = 0;
  
  m_eLockKind = LK_NONE;

  m_ziReadGrabs.vCheckInit();
  m_ziReadCollisions.vCheckInit();
  
  m_ziWriteGrabs.vCheckInit();
  m_ziWriteCollisions.vCheckInit();
  
  _checkInitialised_(CN_LOCK);
  
  _postcondition_(_MAGIC_,CN_LOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Lock::Lock(eLK eLockKind)
: Object(CN_LOCK)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  m_uLockWord = 0;
  
  m_eLockKind = eLockKind;
  
  m_ziReadGrabs.vCheckInit();
  m_ziReadCollisions.vCheckInit();
  
  m_ziWriteGrabs.vCheckInit();
  m_ziWriteCollisions.vCheckInit();
  
  _checkInitialised_(CN_LOCK);
  
  _postcondition_(_MAGIC_,CN_LOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Lock::~Lock(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drop read lock                                                             */
/*----------------------------------------------------------------------------*/
u8 Lock::iLockDropRead(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  //u8 uResult = iDropRead(pWord);
  
  bool b;
  
  do {
    u8 uCurrent = *pWord;
    
    // must have a read lock
    assert(uCurrent && uCurrent != C_WRITELOCK);
    
    u8 uRemoveReadLock = uCurrent - 1;
    
    b = Atomic::bTestAndSet(pWord,uCurrent,uRemoveReadLock);
    
  } while ( ! b );
  
  u8 uResult = 1;
  
#else
  u8 uResult = iDropRead(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* grab read lock                                                             */
/*----------------------------------------------------------------------------*/
u8 Lock::iLockGrabRead(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  
  //u8 uResult = iGrabRead(pWord);
  
  u8 uCurrent = *pWord;
  if (uCurrent >= C_WRITELOCK)
    return 0;
  
  u8 uAddReadLock = uCurrent + 1;
  
  bool b = Atomic::bTestAndSet(pWord,uCurrent,uAddReadLock);
  
  u8 uResult = b ? 1 : 0;
  
#else
  u8 uResult = iGrabRead(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
u8 Lock::iLockDropWrite(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  //u8 uResult = iDropWrite(pWord);
  
  u8 uCurrent = *pWord;
  
  assert (uCurrent == C_WRITELOCK);
  
  bool b = Atomic::bTestAndSet(pWord,uCurrent,0);
  
  u8 uResult = 1;
#else
  u8 uResult = iDropWrite(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
u8 Lock::iLockGrabWrite(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  //u8 uResult = iGrabWrite(pWord);
  
  u8 uCurrent = *pWord;
  
  if (uCurrent >= C_WRITELOCK)
    return 0;
  
  u8 uAddWriteLock = uCurrent + C_WRITELOCK;
  
  bool b = Atomic::bTestAndSet(pWord,uCurrent,uAddWriteLock);
  if ( ! b )
    return 0;
  
  uCurrent = C_WRITELOCK;
  
  while (! Atomic::bTestAndSet(pWord,uCurrent,uCurrent))
    {;}
  
  u8 uResult = 1;
  
#else
  u8 uResult = iGrabWrite(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* is lock actually locked                                                    */
/*----------------------------------------------------------------------------*/
u8 Lock::iLockIsLocked(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  u8 uResult = *pWord ? 1 : 0;
#else
  u8 uResult = iIsLocked(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set lock kind                                                              */
/*----------------------------------------------------------------------------*/
void Lock::vInitLock(eLK eLockKind)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(0 == m_uLockWord); // logic check
  
  m_eLockKind = eLockKind;
}

/*----------------------------------------------------------------------------*/
/* bIsLocked: is write locked                                                 */
/*----------------------------------------------------------------------------*/
bool Lock::bIsLocked(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return Lock::iLockIsLocked(&m_uLockWord);
}

/*----------------------------------------------------------------------------*/
/* uLockWord: is write locked                                                 */
/*----------------------------------------------------------------------------*/
u8 Lock::uLockWord(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return m_uLockWord;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* try and grab read lock                                                     */
/*----------------------------------------------------------------------------*/
bool Lock::bGrabRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);
  
  ++m_ziReadGrabs;
  ++gpAttachMap->m_oLockTracker.m_ziReadGrabs[m_eLockKind];
  
  i8 rCode = Lock::iLockGrabRead(&m_uLockWord);
  if (rCode) {
    vCheckRead();
    return true;
  }
  
  ++m_ziReadCollisions;
  ++gpAttachMap->m_oLockTracker.m_ziReadCollisions[m_eLockKind];
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* vGrabRead: grab read lock                                                  */
/*----------------------------------------------------------------------------*/
void Lock::vGrabRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);
  
  ++m_ziReadGrabs;
  ++gpAttachMap->m_oLockTracker.m_ziReadGrabs[m_eLockKind];
  
  i8 rCode = Lock::iLockGrabRead(&m_uLockWord);
  if (rCode) {
    vCheckRead();
    return;
  }
  
  ++m_ziReadCollisions;
  ++gpAttachMap->m_oLockTracker.m_ziReadCollisions[m_eLockKind];
  
  Timer oTimer;
  oTimer.vStart();
  bool bReported = false;
  
  do {
    oTimer.vUpdate();
    if (oTimer.iMilliseconds() > 100 && ! bReported) {
      bReported = true;
      vWarn("Lock::vGrabRead -- Unable to grab read lock %p %s %p\n"_Stz,
             this,
             aLK[m_eLockKind],
             m_uLockWord);
    }
    
    rCode = Lock::iLockGrabRead(&m_uLockWord);
  } while ( ! rCode );
  
  if (bReported) {
    vWarn("Lock::vGrabRead -- Able to grab read lock %p %p\n"_Stz,
           this, m_uLockWord);
  }
  
  vCheckRead();
}

/*----------------------------------------------------------------------------*/
/* check for read lock                                                        */
/*----------------------------------------------------------------------------*/
void Lock::vCheckRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_uLockWord > 0 && m_uLockWord != C_WRITELOCK); // logic check
}

/*----------------------------------------------------------------------------*/
/* drop read lock                                                             */
/*----------------------------------------------------------------------------*/
void Lock::vDropRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  vCheckRead();
  
  i8 rCode = Lock::iLockDropRead(&m_uLockWord);
  ASSERT(rCode); // logic check
  
  // m_iValue could be any value
}

/*----------------------------------------------------------------------------*/
/* get number of read grabs                                                   */
/*----------------------------------------------------------------------------*/
i8 Lock::iReadGrabs(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ziReadGrabs;
}

/*----------------------------------------------------------------------------*/
/* get number of read collisions                                              */
/*----------------------------------------------------------------------------*/
i8 Lock::iReadCollisions(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ziReadCollisions;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* try and grab write lock                                                    */
/*----------------------------------------------------------------------------*/
bool Lock::bGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);

  ++m_ziWriteGrabs;
  ++gpAttachMap->m_oLockTracker.m_ziWriteGrabs[m_eLockKind];

  i8 rCode = Lock::iLockGrabWrite(&m_uLockWord);
  if (rCode) {
    vCheckWrite();
    return true;
  }
  
  ++m_ziWriteCollisions;
  ++gpAttachMap->m_oLockTracker.m_ziWriteCollisions[m_eLockKind];
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void Lock::vGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);
  
//  vTrace("Lock::vGrabWrite -- Grabbing write lock %p %s\n"_Stz,
//         this,
//         m_aName ? m_aName : "<noName>"_Stz);

  ++m_ziWriteGrabs;
  ++gpAttachMap->m_oLockTracker.m_ziWriteGrabs[m_eLockKind];
  
  i8 rCode = Lock::iLockGrabWrite(&m_uLockWord);
  if (rCode) {
    vCheckWrite();
    return;
  }
  
  ++m_ziWriteCollisions;
  ++gpAttachMap->m_oLockTracker.m_ziWriteCollisions[m_eLockKind];
  
  Timer oTimer;
  oTimer.vStart();
  bool bReported = false;
  
  do {
    oTimer.vUpdate();
    if (oTimer.iMilliseconds() > 100 && ! bReported) {
      bReported = true;
      ActivityMap::vMark(0,"Lock::vGrabWrite -- Unable to grab write lock %p %s %p\n"_Stz,
                         this,
                         aLK[m_eLockKind],
                         m_uLockWord);
      //vWarn("Lock::vGrabWrite -- Unable to grab write lock %p %s %p\n"_Stz,
      //       this,
      //        aLK[m_eLockKind],
      //       m_uLockWord);
    }
    
    rCode = Lock::iLockGrabWrite(&m_uLockWord);
  } while ( ! rCode );
  
  if (bReported) {
    ActivityMap::vMark(0,"Lock::vGrabWrite -- Able to grab write lock %p %p\n"_Stz,
                       this,
                       aLK[m_eLockKind],
                       m_uLockWord);
    //vWarn("Lock::vGrabWrite -- Able to grab write lock %p %p\n"_Stz,
    //       this, m_uLockWord);
  }

  vCheckWrite();
}

/*----------------------------------------------------------------------------*/
/* check for write lock                                                       */
/*----------------------------------------------------------------------------*/
void Lock::vCheckWrite(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_uLockWord >= C_WRITELOCK); // logic check
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void Lock::vDropWrite(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  vCheckWrite();
  
  i8 rCode = Lock::iLockDropWrite(&m_uLockWord);
  ASSERT(rCode); // logic check
  
  //m_uLockWord = 0;
  
  // m_iValue could be any value
}

/*----------------------------------------------------------------------------*/
/* get number of write grabs                                                  */
/*----------------------------------------------------------------------------*/
i8 Lock::iWriteGrabs(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ziWriteGrabs;
}

/*----------------------------------------------------------------------------*/
/* get number of write collisions                                             */
/*----------------------------------------------------------------------------*/
i8 Lock::iWriteCollisions(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ziWriteCollisions;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Lock::aFooter(void)
{
  _precondition_(_MAGIC_,CN_LOCK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
