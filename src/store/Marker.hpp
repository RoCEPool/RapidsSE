/*============================================================================*/
/* Marker.hpp -- Marker                                                       */
/*============================================================================*/
#pragma once

#include "eOM.h"

namespace STORE
{
  class Marker : public Operand
  {
  protected:
    eOM  m_eOpMarker;
    
    Marker* m_pNextItem;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, Job* pJob);
    void operator delete(void*);
  
    Marker(void);
    Marker(eOM eOpMarker);
   ~Marker(void) noexcept(false);
    
    static bool bInspect(Object* pObject);
  
    static void vPostRecv(void* aMarker);

    Marker* pNextItem(void);
    void vNextItem(Marker* pNext);

    eOM eOpMarker(void);
  };

  Marker* _pMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
