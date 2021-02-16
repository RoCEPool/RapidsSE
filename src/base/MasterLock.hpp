/*============================================================================*/
/* MasterLock.hpp -- Data Definition Lock                                        */
/*============================================================================*/
#pragma once

namespace STORE
{
  class MasterLock : public Object
  {
  private:
    Lock  m_okLock;
  
    Task* m_pHoldingTask;
        
  public:
    
    MasterLock(eCN eClassNo, eLK eLock);
   ~MasterLock(void) noexcept(false);
    
  
    void vDropRead(void);
    bool bGrabRead(void);
    void vGrabRead(void);

    void vDropWrite(void);
    bool bGrabWrite(void);
    void vGrabWrite(void);

  private:
    Footer* aFooter(void);
  };
  
  MasterLock* _pMasterLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
