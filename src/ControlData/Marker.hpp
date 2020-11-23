/*============================================================================*/
/* Marker.hpp -- Marker                                                       */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class Marker : public Operand
  {
  protected:
    eOM  m_eOpMarker;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    Marker(void);
    Marker(eOM eOpMarker);
   ~Marker(void) noexcept(false);
    
    eOM eOpMarker(void);
  };

  Marker* _pMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
