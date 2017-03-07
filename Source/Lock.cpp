/*============================================================================*/
/* Lock.cpp -- Shared Read / Exclusive Write Lock                             */
/*============================================================================*/

#include "System.h"
#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Lock* System::_pLock(void* aAddr)
{
  Lock* pLock = reinterpret_cast<Lock*>(aAddr);
  
  RASSERT( !pLock || CN_LOCK == pLock->eClass() );
  
  return pLock;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Lock::operator new(size_t iSize, Lock* pLock)
{
  RASSERT(iSize);
  
  return pLock;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Lock::operator delete(void* pLockAddress)
{
  ASSERT(pLockAddress);
  
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
  
  m_aName = nullptr;

  m_zReadGrabs.vCheckInit();
  m_zReadCollisions.vCheckInit();
  
  m_zWriteGrabs.vCheckInit();
  m_zWriteCollisions.vCheckInit();
  
  _checkInitialised_(CN_LOCK);
  
  _postcondition_(_MAGIC_,CN_LOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Lock::~Lock(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set name of lock                                                           */
/*----------------------------------------------------------------------------*/
void Lock::vInit(Str* aName)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  m_aName = aName;
  
  ASSERT(0 == m_uLockWord); // logic check
}

/*----------------------------------------------------------------------------*/
/* bIsLocked: is write locked                                                 */
/*----------------------------------------------------------------------------*/
bool Lock::bIsLocked(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return Synch::iLockIsLocked(&m_uLockWord);
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
  
  ++m_zReadGrabs;
  
  i8 rCode = Synch::iLockGrabRead(&m_uLockWord);
  if (rCode) {
    vCheckRead();
    return true;
  }
  
  ++m_zReadCollisions;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* vGrabRead: grab read lock                                                  */
/*----------------------------------------------------------------------------*/
void Lock::vGrabRead(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_aName);
  
  ++m_zReadGrabs;
  
  i8 rCode = Synch::iLockGrabRead(&m_uLockWord);
  if (rCode) {
    vCheckRead();
    return;
  }
  
  ++m_zReadCollisions;
  
  Timer xTimer;
  xTimer.vStart();
  bool bReported = false;
  
  do {
    xTimer.vUpdate();
    if (xTimer.iMilliseconds() > 100 && ! bReported) {
      bReported = true;
      vTrace("Lock::vGrabRead -- Unable to grab read lock %p %s\n"_Str,
             this,
             m_aName ? m_aName : "<noName>"_Str);
    }
    
    rCode = Synch::iLockGrabRead(&m_uLockWord);
  } while ( ! rCode );
  
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
  
  i8 rCode = Synch::iLockDropRead(&m_uLockWord);
  ASSERT(rCode); // logic check
  
  // m_iValue could be any value
}

/*----------------------------------------------------------------------------*/
/* get number of read grabs                                                   */
/*----------------------------------------------------------------------------*/
i8 Lock::iReadGrabs(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_zReadGrabs;
}

/*----------------------------------------------------------------------------*/
/* get number of read collisions                                              */
/*----------------------------------------------------------------------------*/
i8 Lock::iReadCollisions(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_zReadCollisions;
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
  
  ++m_zWriteGrabs;
  
  i8 rCode = Synch::iLockGrabWrite(&m_uLockWord);
  if (rCode) {
    vCheckWrite();
    return true;
  }
  
  ++m_zWriteCollisions;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void Lock::vGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_aName);

//  vTrace("Lock::vGrabWrite -- Grabbing write lock %p %s\n"_Str,
//         this,
//         m_aName ? m_aName : "<noName>"_Str);

  ++m_zWriteGrabs;
  i8 rCode = Synch::iLockGrabWrite(&m_uLockWord);
  if (rCode) {
    vCheckWrite();
    return;
  }
  
  ++m_zWriteCollisions;
  
  Timer xTimer;
  xTimer.vStart();
  bool bReported = false;
  
  do {
    xTimer.vUpdate();
    if (xTimer.iMilliseconds() > 100 && ! bReported) {
      bReported = true;
      vTrace("Lock::vGrabWrite -- Unable to grab write lock %p %s\n"_Str,
             this,
             m_aName ? m_aName : "<noName>"_Str);
    }
    
    rCode = Synch::iLockGrabWrite(&m_uLockWord);
  } while ( ! rCode );
  
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
  
  i8 rCode = Synch::iLockDropWrite(&m_uLockWord);
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
  
  return *m_zWriteGrabs;
}

/*----------------------------------------------------------------------------*/
/* get number of write collisions                                             */
/*----------------------------------------------------------------------------*/
i8 Lock::iWriteCollisions(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return *m_zWriteCollisions;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Lock::pFooter(void)
{
  _precondition_(_MAGIC_,CN_LOCK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
