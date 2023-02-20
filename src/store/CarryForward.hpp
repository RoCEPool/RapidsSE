/*============================================================================*/
/* CarryForward.hpp -- Carry Forward                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class CarryForward : public Object
  {
  private:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CarryForward(void);
   ~CarryForward(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  CarryForward* _pCarryForward(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
