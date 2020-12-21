/*============================================================================*/
/* PoolSnapshot.hpp - objects in the buffer pool                             */
/*============================================================================*/
#pragma once

namespace Resources
{
  
  class PoolSnapshot : public Object
  {
  
  public:
    i8  m_nObjects[CN_ENUMS];

    // must be last
    Footer m_xFooter;
    
  public:        
    void* operator new(size_t);
    void  operator delete(void* pAddr);
    
    PoolSnapshot(void);
   ~PoolSnapshot(void) noexcept(false);
    
  private:

    Footer* aFooter(void);
   };
  
  PoolSnapshot* _pObjectsInPool(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
