/*============================================================================*/
/* TimeMarker.hpp -- Time Marker operand                                      */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class TimeMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TimeMarker(void);
   ~TimeMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  TimeMarker* _pTimeMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
