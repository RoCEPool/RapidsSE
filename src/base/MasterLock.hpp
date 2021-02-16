/*============================================================================*/
/* MasterLock.hpp -- Data Definition Lock                                        */
/*============================================================================*/
#pragma once

namespace STORE
{
  class MasterLock : public Object
  {
  private:
    Lock m_okLock;
        
  public:
    
    MasterLock(eCN eClassNo, eLK eLock);
   ~MasterLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  MasterLock* _pMasterLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
