/*============================================================================*/
/* Marker.hpp -- Marker                                                       */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class Marker : public Operand
  {
  protected:
  
  public:
    void operator delete(void*);
  
    Marker(eCN eClass);
   ~Marker(void) noexcept(false);
  };

  Marker* _pMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
