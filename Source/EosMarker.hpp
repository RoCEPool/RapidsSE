/*============================================================================*/
/* EosMarker.hpp -- End Marker operand                                        */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class EosMarker : public Operand
  {
  public:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    EosMarker(void);
   ~EosMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  EosMarker* _pEosMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
