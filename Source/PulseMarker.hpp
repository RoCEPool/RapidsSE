/*============================================================================*/
/* PulseMarker.hpp -- Pulse Marker operand                                    */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class PulseMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PulseMarker(void);
   ~PulseMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  PulseMarker* _pPulseMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
