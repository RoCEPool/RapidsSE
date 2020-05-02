/*============================================================================*/
/* Lock.cpp -- Shared Read / Exclusive Write Lock                             */
/*============================================================================*/

#include "Locking.h"


#include "Timer.hpp"

Atomic gixLock;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Lock* Locking::_pLock(void* aAddr)
{
  Lock* pLock = reinterpret_cast<Lock*>(aAddr);
  
  RASSERT_TYPE(pLock, CN_LOCK);
  
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
/* constructor (for non-focal case)                                           */
/*----------------------------------------------------------------------------*/
Lock::Lock(eLK eLockKind, Object* pFor)
: Focal(CN_LOCK)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
//  if (pFor) {
//    printf("  Lock(%s)\n",(char*)aCN[pFor->eClass()]);
//    AM::vPrintGlobals();
//  }
//  else {
//    AM::vPrintGlobals();
//    vDebugPoint();
//  }
  
  m_pFor   = pFor;
  m_bFocal = false;
  
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
 
  if (m_bFocal) {
    SP_Gid spId;
    
    vIncStepNo(); -- gixLock;
    AM::vLogLck(SL_INF,this,LA_LCK_DESTRUCT,
                "Lock -%s destruct ^For=%s |gixLock=%lld|"_Stz, //!format-LogLck
                aLK[m_eLockKind],
                spId.sz(m_pFor?((Focal*)m_pFor)->m_iFocalId:C_UNKNOWN),
                * gixLock);
  }
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
/* is the lock currently locked?                                              */
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
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Lock::vFocalOutline(s4K& sOutline)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
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
/* get lock id                                                                */
/*----------------------------------------------------------------------------*/
i8 Lock::iLockId(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return m_iFocalId;
}

/*----------------------------------------------------------------------------*/
/* get the kind of lock                                                       */
/*----------------------------------------------------------------------------*/
eLK Lock::eLockKind(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return m_eLockKind;
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
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixReadGrabs[m_eLockKind];
#endif
  
  i8 iResult = Lock::iLockGrabRead(&m_uLockWord);
  if (iResult) {
    vAssertRead();
    return true;
  }
  
  ++m_ixReadCollisions;
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixReadCollisions[m_eLockKind];
#endif
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* grab a read lock (may block)                                               */
/*----------------------------------------------------------------------------*/
void Lock::vGrabRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // attempt to grab the lock for read
  
  ++m_ixReadGrabs;
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixReadGrabs[m_eLockKind];
#endif
  
  i8 iResult = Lock::iLockGrabRead(&m_uLockWord);
  if (iResult) {
    vAssertRead();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collision with a writer; spin till read lock is acquired

  ++m_ixReadCollisions;
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixReadCollisions[m_eLockKind];
#endif
  
  Timer oTimer;
  oTimer.vStart();
  
  bool bReported = false;
  while (! Lock::iLockGrabRead(&m_uLockWord) ) {

    if (! m_pFor || CN_LOG == m_pFor->eClass())
      continue;
    
    oTimer.vUpdate();
    if (oTimer.iMilliseconds() < 100 || bReported)
      continue;
    
    bReported = true;
    vIncStepNo();
    AM::vLogLck(SL_INF,this,LA_LCK_READ_UNABLE,
                "Unable to grab read lock %p %s %p"_Stz, //!format-LogLck
                this,
                aLK[m_eLockKind],
                 m_uLockWord);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read lock has been acquired

  if (bReported) {
    vIncStepNo();
    AM::vLogLck(SL_INF,this,LA_LCK_READ_ABLE,
               "Able to grab read lock %p %s %p"_Stz, //!format-LogLck
                this,
                aLK[m_eLockKind],
                m_uLockWord);
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
  RASSERT(iResult); // logic check
  
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
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixWriteGrabs[m_eLockKind];
#endif
  
  i8 iResult = Lock::iLockGrabWrite(&m_uLockWord);
  if (iResult) {
    vAssertWrite();
    return true;
  }
  
  ++m_ixWriteCollisions;
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixWriteCollisions[m_eLockKind];
#endif
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* grab write lock (may block)                                                */
/*----------------------------------------------------------------------------*/
void Lock::vGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_eLockKind);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // attempt to grab the lock for write
  
  ++m_ixWriteGrabs;
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixWriteGrabs[m_eLockKind];
#endif
  
  i8 iResult = Lock::iLockGrabWrite(&m_uLockWord);
  if (iResult) {
    vAssertWrite();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collision with another writer; spin till write lock is acquired
  
  ++m_ixWriteCollisions;
#ifdef WITH_LOCK_TRACKING
  ++AM::pLockTracker()->m_ixWriteCollisions[m_eLockKind];
#endif
  
  Timer oTimer;
  oTimer.vStart();
  
  bool bReported = false;
  while (! Lock::iLockGrabWrite(&m_uLockWord)) {

    if (! m_pFor || CN_LOG == m_pFor->eClass())
      continue;
    
    oTimer.vUpdate();
    if (oTimer.iMilliseconds() < 100 || bReported)
      continue;
    
    bReported = true;
    vIncStepNo();
    AM::vLogLck(SL_INF,this,LA_LCK_WRITE_UNABLE,
                "Unable to grab write lock %p %s %p"_Stz, //!format-LogLck
                this,
                aLK[m_eLockKind],
                m_uLockWord);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write lock has been acquired

  if (bReported) {
    vIncStepNo();
    AM::vLogLck(SL_INF,this,LA_LCK_WRITE_ABLE,
               "Able to grab write lock %p %s %p"_Stz, //!format-LogLck
                       this,
                       aLK[m_eLockKind],
                       m_uLockWord);
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
__aFooterFunction(Lock)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
