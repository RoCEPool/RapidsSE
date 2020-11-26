/*============================================================================*/
/* Lock.hpp -- Shared Read / Exclusive Write Lock                             */
/*============================================================================*/
#pragma once

namespace Locking
{
  const u8 C_WRITELOCK = 0x8000000000000000;
    
  class Lock : public Focal
  {
  private:
    eLK      m_eLockKind;         // lock kind
    Object*  m_pFor;
    bool     m_bFocal;            // is a lock for a focal object
  
    u8       m_uLockWord;         // lock word
  
    Atomic   m_ixReadGrabs;       // number of read grabs
    Atomic   m_ixReadCollisions;  // number of read collisions
    
    Atomic   m_ixWriteGrabs;      // number of write grabs
    Atomic   m_ixWriteCollisions; // number of write collisions
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Lock* pLock);
    void  operator delete(void* pLockAddress);
    
    Lock(eLK eLockKind, Object* pFor);
   ~Lock(void) noexcept(false);
  
  private:
    static u8 iLockDropRead(u8*);
    static u8 iLockGrabRead(u8*);
    static u8 iLockDropWrite(u8*);
    static u8 iLockGrabWrite(u8*);
    static u8 iLockIsLocked(u8*);

  public:
    void vFocalOutline(s4K& sOutline);
  
    bool bIsLocked(void);
    i8   iLockId(void);
    eLK  eLockKind(void);
    u8   uLockWord(void);
  
    void vAssertRead(void);
    void vDropRead(void);
    bool bGrabRead(void);
    void vGrabRead(void);
    i8   iReadGrabs(void);
    i8   iReadCollisions(void);
    
    void vAssertWrite(void);
    void vDropWrite(void);
    bool bGrabWrite(void);
    void vGrabWrite(void);
    bool bIsWriteLocked(void);
    i8   iWriteGrabs(void);
    i8   iWriteCollisions(void);
    
  private:
    Footer* aFooter(void);
  };
  
  Lock* _pLock(void* aAddr);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
