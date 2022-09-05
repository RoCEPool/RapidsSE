/*============================================================================*/
/* MarkerChain.hpp -- Chain of Free Markers                                   */
/*============================================================================*/
#pragma once

#include "Chain_t.hpp"
#include "Marker.hpp"

namespace STORE
{
  class MarkerChain : public Chain_t<CN_MARKER,Marker>
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MarkerChain(void);
   ~MarkerChain(void) noexcept(false);
   
    Footer* aFooter(void);
  };
  
  MarkerChain* _pMarkerChain(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
