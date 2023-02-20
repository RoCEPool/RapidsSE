/*============================================================================*/
/* Smooth.hpp -- Smooth                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class Smooth : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Smooth(void);
   ~Smooth(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Smooth* _pSmooth(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
