/*============================================================================*/
/* StartedMarker.hpp -- Started Marker operand                                */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class StartedMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StartedMarker(void);
   ~StartedMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  StartedMarker* _pStartedMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
