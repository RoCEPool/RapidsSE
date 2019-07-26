/*============================================================================*/
/* CancelMarker.hpp -- Cancel Marker operand                                  */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class CancelMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CancelMarker(void);
   ~CancelMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  CancelMarker* _pCancelMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
