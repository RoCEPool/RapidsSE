/*============================================================================*/
/* Lock.cpp -- Lock                                                           */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  const u8 C_WRITELOCK = 0x8000000000000000;
  
  class Lock : public Header
  {
  private:
    u8     m_uLockWord;        // lock word
    
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
    
    void vCheckInit(void);
    
    bool bGrabRead(void);
    void vGrabRead(void);
    void vCheckRead(void);
    void vDropRead(void);
    
    bool bGrabWrite(void);
    void vGrabWrite(void);
    void vCheckWrite(void);
    void vDropWrite(void);
    
    bool bIsLocked(void);
    
    
    Footer* pFooter(void);
  };
  
  Lock* _pLock(void* pAddr);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
