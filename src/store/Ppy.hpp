/*============================================================================*/
/* Ppy.hpp -- property                                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class Ppy : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Ppy(void);
   ~Ppy(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };

  Ppy* _pPpy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
