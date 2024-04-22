/*============================================================================*/
/* AbstractDef.hpp -- Abstract Definition                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class AbstractDef : public Object
  {
  public:

  public:
    void* operator new(size_t);
    void  operator delete(void*);

     AbstractDef(eCN eClass);
    ~AbstractDef(void) noexcept(false);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
