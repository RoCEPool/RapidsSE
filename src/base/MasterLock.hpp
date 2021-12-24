/*============================================================================*/
/* MasterLock.hpp -- Master Lock                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class MasterLock : public Object
  {
  private:
    Lock  m_okLock;
  
    Task* m_pHoldingTask;
        
  public:
    
    MasterLock(eCN eClassNo);
   ~MasterLock(void) noexcept(false);
    
    u8   uLockWord(void);

    void vDropRead(void);
    bool bGrabRead(void);
    void vGrabRead(void);

    void vDropWrite(void);
    bool bGrabWrite(void);
    void vGrabWrite(void);
    bool bIsWriteLocked(void);
    void vResetStats(void);

    i8   iReadGrabs(void);
    i8   iReadCollisions(void);
    
    i8   iWriteGrabs(void);
    i8   iWriteCollisions(void);
  };
  
  MasterLock* _pMasterLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
