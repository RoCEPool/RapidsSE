/*============================================================================*/
/* PpySet.hpp -- property set                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ppy.hpp"

namespace STORE
{
  class PpySet : public Object
  {
  private:
    static const i8 C_MAX_PPY = 8;

    Ppy m_oaPpy[C_MAX_PPY];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PpySet(void);
   ~PpySet(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };

  PpySet* _pPpySet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
