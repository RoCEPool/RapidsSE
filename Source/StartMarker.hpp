/*============================================================================*/
/* StartMarker.hpp -- Start Marker operand                                    */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class StartMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StartMarker(void);
   ~StartMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  StartMarker* _pStartMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
