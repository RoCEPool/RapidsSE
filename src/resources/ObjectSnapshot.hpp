/*============================================================================*/
/* ObjectSnapshot.hpp -- count of objects by class in the buffer pool         */
/*============================================================================*/
#pragma once

namespace Resources
{
  
  class ObjectSnapshot : public Object
  {
  
  public:
  
    i8  m_iWhenSnapshot;
  
    i8  m_nObjects[CN_ENUMS];

    // must be last
    Footer m_xFooter;
    
  public:        
    void* operator new(size_t);
    void  operator delete(void* pAddr);
    
    ObjectSnapshot(void);
   ~ObjectSnapshot(void) noexcept(false);
  
    void vClassCounts(void);

    void vTakeSnapshot(void);

  private:

    Footer* aFooter(void);
   };
  
  ObjectSnapshot* _pObjectSnapshot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
