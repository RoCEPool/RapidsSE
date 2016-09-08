/*============================================================================*/
/* Lock.cpp -- locking                                                        */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  class Lock : public Header
  {
  private:
    u8 m_uValue;
    
  public:
    void* operator new(size_t, Lock* pLock);
    void  operator delete(void* pLockAddress);
    
    Lock(void);
    ~Lock(void);
    
    void vInit(void);
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
    
    // must be last
    Footer m_xFooter;
    
    Footer* pFooter(void);
  };
  
  const u8 C_WRITELOCK = 0x8000000000000000;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
