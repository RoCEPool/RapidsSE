/*============================================================================*/
/* ErrorMarker.hpp -- Error Marker operand                                    */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class ErrorMarker : public Operand
  {
  public:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ErrorMarker(void);
   ~ErrorMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  ErrorMarker* _pErrorMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
