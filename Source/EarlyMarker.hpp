/*============================================================================*/
/* EarlyMarker.hpp -- Early Marker operand                                    */
/*============================================================================*/
#pragma once

namespace ControlData
{
  class EarlyMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    EarlyMarker(void);
   ~EarlyMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  EarlyMarker* _pEarlyMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
