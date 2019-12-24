/*============================================================================*/
/* CompletedMarker.hpp -- Stop Marker operand                                      */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class CompletedMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CompletedMarker(void);
   ~CompletedMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  CompletedMarker* _pCompletedMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
