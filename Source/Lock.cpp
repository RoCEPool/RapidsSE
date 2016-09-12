/*============================================================================*/
/* Lock.cpp -- locking                                                        */
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
  ASSERT(iSize);
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

//---------------------------------------------------------------------------//
// Lock: constructor                                                         //
//---------------------------------------------------------------------------//
Lock::Lock(void)
: Header(CN_LOCK)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  vInit();
  
  Memory::vCheckInitialised(this,sizeof(Lock));
  
  _postcondition_(_MAGIC_,CN_LOCK,_FOOTER_);
}

//---------------------------------------------------------------------------//
// ~Lock: destructor                                                         //
//---------------------------------------------------------------------------//
Lock::~Lock(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
}

//---------------------------------------------------------------------------//
// init                                                                      //
//---------------------------------------------------------------------------//
void Lock::vInit(void)
{  
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  m_uValue = 0;
}

//---------------------------------------------------------------------------//
// vCheckInit: check lock initialization                                     //
//---------------------------------------------------------------------------//
void Lock::vCheckInit()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(0 == m_uValue); // logic check
}

//---------------------------------------------------------------------------//
// vCheckRead: check for read lock                                           //
//---------------------------------------------------------------------------//
void Lock::vCheckRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_uValue > 0 && m_uValue != C_WRITELOCK); // logic check
}

//---------------------------------------------------------------------------//
// vCheckWrite: check for write lock                                         //
//---------------------------------------------------------------------------//
void Lock::vCheckWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  ASSERT(m_uValue >= C_WRITELOCK); // logic check
}

//---------------------------------------------------------------------------//
// vDropRead: drop read lock                                                 //
//---------------------------------------------------------------------------//
void Lock::vDropRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  vCheckRead();
  
  i8 rCode = iDropRead(&m_uValue);
  ASSERT(rCode); // logic check
  
  // m_iValue could be any value
}

//---------------------------------------------------------------------------//
// vDropWrite: drop write lock                                               //
//---------------------------------------------------------------------------//
void Lock::vDropWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  vCheckWrite();
  
  i8 rCode = iDropWrite(&m_uValue);
  ASSERT(rCode); // logic check
  
  //m_uValue = 0;
  
  // m_iValue could be any value
}

//---------------------------------------------------------------------------//
// bGrabRead: try and grab read lock                                         //
//---------------------------------------------------------------------------//
bool Lock::bGrabRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  i8 rCode = iGrabRead(&m_uValue);
  
  if (rCode) {
    vCheckRead();
    return true;
  }
  
  return false;
}

//---------------------------------------------------------------------------//
// vGrabRead: grab read lock                                                 //
//---------------------------------------------------------------------------//
void Lock::vGrabRead()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  i8 rCode;
  
  do {
    rCode = iGrabRead(&m_uValue);
  } while ( ! rCode );
  
  vCheckRead();
}

//---------------------------------------------------------------------------//
// bGrabWrite: try and grab write lock                                       //
//---------------------------------------------------------------------------//
bool Lock::bGrabWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  i8 rCode = iGrabWrite(&m_uValue);
  
  if (rCode) {
    vCheckWrite();
    return true;
  }
  
  return false;
}

//---------------------------------------------------------------------------//
// vGrabWrite: grab write lock                                               //
//---------------------------------------------------------------------------//
void Lock::vGrabWrite()
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
  i8 rCode;
  
  do {
    rCode = iGrabWrite(&m_uValue);
  } while ( ! rCode );
  
  vCheckWrite();
}

//---------------------------------------------------------------------------//
// bIsLocked: is write locked                                                //
//---------------------------------------------------------------------------//
bool Lock::bIsLocked(void)
{
  _precondition_(_MAGIC_,CN_LOCK,_FOOTER_);
  
    return iIsLocked(&m_uValue);
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