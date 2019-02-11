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

  m_ixReadGrabs.vCheckInit();
  m_ixReadCollisions.vCheckInit();
  
  m_ixWriteGrabs.vCheckInit();
  m_ixWriteCollisions.vCheckInit();
  
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
  
  m_ixReadGrabs.vCheckInit();
  m_ixReadCollisions.vCheckInit();
  
  m_ixWriteGrabs.vCheckInit();
  m_ixWriteCollisions.vCheckInit();
  
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
#if defined(WITH_APPLE) || defined(WITH_LINUX)
  //u8 uResult = iDropRead(pWord);
  
  bool bWasSet;
  
  do {
    u8 uCurrent = *pWord;
    
    // must have a read lock
    assert(uCurrent && uCurrent != C_WRITELOCK);
    
    u8 uRemoveReadLock = uCurrent - 1;
    
    bWasSet = Atomic::bTestAndSet(pWord,uCurrent,uRemoveReadLock);
    
  } while ( ! bWasSet );
  
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
#if defined(WITH_APPLE) || defined(WITH_LINUX)
  
  //u8 uResult = iGrabRead(pWord);
  
  u8 uCurrent = *pWord;
  if (uCurrent >= C_WRITELOCK)
    return 0;
  
  u8 uAddReadLock = uCurrent + 1;
  
  bool bWasSet = Atomic::bTestAndSet(pWord,uCurrent,uAddReadLock);
  
  u8 uResult = bWasSet ? 1 : 0;
  
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
#if defined(WITH_APPLE) || defined(WITH_LINUX)
  //u8 uResult = iDropWrite(pWord);
  
  u8 uCurrent = *pWord;
  
  assert (uCurrent == C_WRITELOCK);
  
  bool bWasSet = Atomic::bTestAndSet(pWord,uCurrent,0);
  
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
#if defined(WITH_APPLE) || defined(WITH_LINUX)
  //u8 uResult = iGrabWrite(pWord);
  
  u8 uCurrent = *pWord;
  
  if (uCurrent >= C_WRITELOCK)
    return 0;
  
  u8 uAddWriteLock = uCurrent + C_WRITELOCK;
  
  bool bWasSet = Atomic::bTestAndSet(pWord,uCurrent,uAddWriteLock);
  if ( ! bWasSet )
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
/* is the lock currentl locked?                                               */
/*----------------------------------------------------------------------------*/
u8 Lock::iLockIsLocked(u8* pWord)
{
#if defined(WITH_APPLE) || defined(WITH_LINUX)
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
/* is the lock locked?                                                        */
/*----------------------------------------------------------------------------*/
bool Lock::bIsLocked(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return Lock::iLockIsLocked(&m_uLockWord);
}

/*----------------------------------------------------------------------------*/
/* get the value of the lock word                                             */
/*----------------------------------------------------------------------------*/
u8 Lock::uLockWord(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return m_uLockWord;
}


////////////////////////////////////////////////////////////////////////////////
//                                Read Locking                                //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* check for read lock                                                        */
/*----------------------------------------------------------------------------*/
void Lock::vAssertRead(void)
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
  
  vAssertRead();
  
  i8 iResult = Lock::iLockDropRead(&m_uLockWord);
  ASSERT(iResult); // logic check
  
  // m_iValue could be any value
}

/*----------------------------------------------------------------------------*/
/* try and grab a read lock                                                   */
/*----------------------------------------------------------------------------*/
bool Lock::bGrabRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);
  
  ++m_ixReadGrabs;
  ++gpAttachMap->m_oLockTracker.m_ixReadGrabs[m_eLockKind];
  
  i8 iResult = Lock::iLockGrabRead(&m_uLockWord);
  if (iResult) {
    vAssertRead();
    return true;
  }
  
  ++m_ixReadCollisions;
  ++gpAttachMap->m_oLockTracker.m_ixReadCollisions[m_eLockKind];
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* grab a read lock (may block)                                               */
/*----------------------------------------------------------------------------*/
void Lock::vGrabRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);
  
  ++m_ixReadGrabs;
  ++gpAttachMap->m_oLockTracker.m_ixReadGrabs[m_eLockKind];
  
  i8 iResult = Lock::iLockGrabRead(&m_uLockWord);
  if (iResult) {
    vAssertRead();
    return;
  }
  
  ++m_ixReadCollisions;
  ++gpAttachMap->m_oLockTracker.m_ixReadCollisions[m_eLockKind];
  
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
    
    iResult = Lock::iLockGrabRead(&m_uLockWord);
  } while ( ! iResult );
  
  if (bReported) {
    vWarn("Lock::vGrabRead -- Able to grab read lock %p %p\n"_Stz,
           this, m_uLockWord);
  }
  
  vAssertRead();
}

/*----------------------------------------------------------------------------*/
/* get number of read grabs                                                   */
/*----------------------------------------------------------------------------*/
i8 Lock::iReadGrabs(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ixReadGrabs;
}

/*----------------------------------------------------------------------------*/
/* get number of read collisions                                              */
/*----------------------------------------------------------------------------*/
i8 Lock::iReadCollisions(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ixReadCollisions;
}

////////////////////////////////////////////////////////////////////////////////
//                                Write Locking                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* check for write lock                                                       */
/*----------------------------------------------------------------------------*/
void Lock::vAssertWrite(void)
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
  
  vAssertWrite();
  
  i8 iResult = Lock::iLockDropWrite(&m_uLockWord);
  ASSERT(iResult); // logic check
  
  //m_uLockWord = 0;
  
  // m_iValue could be any value
}

/*----------------------------------------------------------------------------*/
/* try and grab write lock                                                    */
/*----------------------------------------------------------------------------*/
bool Lock::bGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);

  ++m_ixWriteGrabs;
  ++gpAttachMap->m_oLockTracker.m_ixWriteGrabs[m_eLockKind];

  i8 iResult = Lock::iLockGrabWrite(&m_uLockWord);
  if (iResult) {
    vAssertWrite();
    return true;
  }
  
  ++m_ixWriteCollisions;
  ++gpAttachMap->m_oLockTracker.m_ixWriteCollisions[m_eLockKind];
  
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

  ++m_ixWriteGrabs;
  ++gpAttachMap->m_oLockTracker.m_ixWriteGrabs[m_eLockKind];
  
  i8 iResult = Lock::iLockGrabWrite(&m_uLockWord);
  if (iResult) {
    vAssertWrite();
    return;
  }
  
  ++m_ixWriteCollisions;
  ++gpAttachMap->m_oLockTracker.m_ixWriteCollisions[m_eLockKind];
  
  Timer oTimer;
  oTimer.vStart();
  bool bReported = false;
  
  do {
    oTimer.vUpdate();
    if (oTimer.iMilliseconds() > 100 && ! bReported) {
      bReported = true;
      ActivityMap::vMarkInfo("Lock::vGrabWrite -- Unable to grab write lock %p %s %p"_Stz,
                         this,
                         aLK[m_eLockKind],
                         m_uLockWord);
      //vWarn("Lock::vGrabWrite -- Unable to grab write lock %p %s %p\n"_Stz,
      //       this,
      //        aLK[m_eLockKind],
      //       m_uLockWord);
    }
    
    iResult = Lock::iLockGrabWrite(&m_uLockWord);
  } while ( ! iResult );
  
  if (bReported) {
    ActivityMap::vMarkInfo("Lock::vGrabWrite -- Able to grab write lock %p %s %p"_Stz,
                       this,
                       aLK[m_eLockKind],
                       m_uLockWord);
    //vWarn("Lock::vGrabWrite -- Able to grab write lock %p %p\n"_Stz,
    //       this, m_uLockWord);
  }

  vAssertWrite();
}

/*----------------------------------------------------------------------------*/
/* is write locked?                                                           */
/*----------------------------------------------------------------------------*/
bool Lock::bIsWriteLocked(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  bool bWriteLocked = (m_uLockWord >= C_WRITELOCK ? true : false);
  
  return bWriteLocked;
}

/*----------------------------------------------------------------------------*/
/* get number of write grabs                                                  */
/*----------------------------------------------------------------------------*/
i8 Lock::iWriteGrabs(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ixWriteGrabs;
}

/*----------------------------------------------------------------------------*/
/* get number of write collisions                                             */
/*----------------------------------------------------------------------------*/
i8 Lock::iWriteCollisions(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_ixWriteCollisions;
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
