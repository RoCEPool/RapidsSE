/*============================================================================*/
/* PauseMarker.hpp -- Pause Marker operand                                    */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class PauseMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PauseMarker(void);
   ~PauseMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  PauseMarker* _pPauseMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
