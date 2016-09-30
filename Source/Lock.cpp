/*============================================================================*/
/* Lock.cpp -- Lock                                                           */
/*============================================================================*/

#include "Common.h"
#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Lock* Common::_pLock(void* pAddr)
{
  Lock* pLock = reinterpret_cast<Lock*>(pAddr);
  
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
: Header(CN_LOCK)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  m_uLockWord = 0;
  
  m_zReadGrabs.vCheckInit();
  m_zReadCollisions.vCheckInit();
  
  m_zWriteGrabs.vCheckInit();
  m_zWriteCollisions.vCheckInit();
  
  _checkInitialised_(sizeof(Lock));
  
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
/* check lock initialization                                                  */
/*----------------------------------------------------------------------------*/
void Lock::vCheckInit()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(0 == m_uLockWord); // logic check
}

/*----------------------------------------------------------------------------*/
/* bIsLocked: is write locked                                                 */
/*----------------------------------------------------------------------------*/
bool Lock::bIsLocked(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  return iIsLocked(&m_uLockWord);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* try and grab read lock                                                     */
/*----------------------------------------------------------------------------*/
bool Lock::bGrabRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ++m_zReadGrabs;
  
  i8 rCode = iGrabRead(&m_uLockWord);
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
void Lock::vGrabRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ++m_zReadGrabs;
  
  i8 rCode = iGrabRead(&m_uLockWord);
  if (rCode) {
    vCheckRead();
    return;
  }
  
  ++m_zReadCollisions;
  
  do {
    rCode = iGrabRead(&m_uLockWord);
  } while ( ! rCode );
  
  vCheckRead();
}

/*----------------------------------------------------------------------------*/
/* check for read lock                                                        */
/*----------------------------------------------------------------------------*/
void Lock::vCheckRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_uLockWord > 0 && m_uLockWord != C_WRITELOCK); // logic check
}

/*----------------------------------------------------------------------------*/
/* drop read lock                                                             */
/*----------------------------------------------------------------------------*/
void Lock::vDropRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  vCheckRead();
  
  i8 rCode = iDropRead(&m_uLockWord);
  ASSERT(rCode); // logic check
  
  // m_iValue could be any value
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* try and grab write lock                                                    */
/*----------------------------------------------------------------------------*/
bool Lock::bGrabWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ++m_zWriteGrabs;
  
  i8 rCode = iGrabWrite(&m_uLockWord);
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
void Lock::vGrabWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ++m_zWriteGrabs;
  i8 rCode = iGrabWrite(&m_uLockWord);
  if (rCode) {
    vCheckWrite();
    return;
  }
  
  ++m_zWriteCollisions;
  
  do {
    rCode = iGrabWrite(&m_uLockWord);
  } while ( ! rCode );
  
  vCheckWrite();
}

/*----------------------------------------------------------------------------*/
/* check for write lock                                                       */
/*----------------------------------------------------------------------------*/
void Lock::vCheckWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_uLockWord >= C_WRITELOCK); // logic check
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void Lock::vDropWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  vCheckWrite();
  
  i8 rCode = iDropWrite(&m_uLockWord);
  ASSERT(rCode); // logic check
  
  //m_uLockWord = 0;
  
  // m_iValue could be any value
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