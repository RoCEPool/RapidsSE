/*============================================================================*/
/* Lock.hpp -- Shared Read / Exclusive Write Lock                             */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  const u8 C_WRITELOCK = 0x8000000000000000;
  
  static const i8 C_HAVE_READ_LOCK  = 1;
  static const i8 C_HAVE_WRITE_LOCK = 2;
  
  class Lock : public Object
  {
  private:
    u8     m_uLockWord;        // lock word
  
    const char* m_aName;       // name
  
    Atomic m_zReadGrabs;       // number of read grabs
    Atomic m_zReadCollisions;  // number of read collisions
    
    Atomic m_zWriteGrabs;      // number of write grabs
    Atomic m_zWriteCollisions; // number of write collisions
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Lock* pLock);
    void  operator delete(void* pLockAddress);
    
    Lock(void);
    ~Lock(void);
  
    void vInit(const char* aName);
  
    bool bIsLocked(void);
    
    bool bGrabRead(void);
    void vGrabRead(void);
    void vCheckRead(void);
    void vDropRead(void);
    i8   iReadGrabs(void);
    i8   iReadCollisions(void);
    
    bool bGrabWrite(void);
    void vGrabWrite(void);
    void vCheckWrite(void);
    void vDropWrite(void);
    i8   iWriteGrabs(void);
    i8   iWriteCollisions(void);
    
    Footer* pFooter(void);
  };
  
  Lock* _pLock(void* pAddr);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
