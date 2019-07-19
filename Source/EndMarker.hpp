/*============================================================================*/
/* EndMarker.hpp -- End Marker operand                                        */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class EndMarker : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    EndMarker(void);
   ~EndMarker(void) noexcept(false);

    Footer* aFooter(void);
  };

  EndMarker* _pEndMarker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
