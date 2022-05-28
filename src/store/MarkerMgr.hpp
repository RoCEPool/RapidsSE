/*============================================================================*/
/* MarkerMgr.hpp -- Marker Manager                                            */
/*============================================================================*/
#pragma once

#include "Marker.hpp"

namespace STORE
{
  class MarkerMgr : public Object
  {
  private:
  
    Marker* m_paMarker[OM_ENUMS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MarkerMgr(void);
   ~MarkerMgr(void) noexcept(false);
  
    Marker* pMarker(eOM eMarker);
  
    Footer* aFooter(void);
  };
  
  MarkerMgr* _pMarkerMgr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
