/*============================================================================*/
/* ObjectsInPool.hpp - objects in the buffer pool                             */
/*============================================================================*/
#pragma once

namespace Resources
{
  
  class ObjectsInPool : public Object
  {
  
  public:
    i8  m_nObjects[CN_ENUMS];

    // must be last
    Footer m_xFooter;
    
  public:        
    void* operator new(size_t);
    void  operator delete(void* pAddr);
    
    ObjectsInPool(void);
   ~ObjectsInPool(void) noexcept(false);
    
  private:

    Footer* aFooter(void);
   };
  
  ObjectsInPool* _pObjectsInPool(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
