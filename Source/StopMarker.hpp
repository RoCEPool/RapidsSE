/*============================================================================*/
/* StopMarker.hpp -- Stop Marker operand                                      */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class StopMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StopMarker(void);
   ~StopMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  StopMarker* _pStopMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
