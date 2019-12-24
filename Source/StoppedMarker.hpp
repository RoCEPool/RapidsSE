/*============================================================================*/
/* StoppedMarker.hpp -- Stopped Marker operand                                */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class StoppedMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StoppedMarker(void);
   ~StoppedMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  StoppedMarker* _pStoppedMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
