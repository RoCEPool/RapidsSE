/*============================================================================*/
/* Proxy.hpp - Proxy                                                          */
/*============================================================================*/

#pragma once

#include "GridModel.h"

namespace GridModel
{
  class Proxy : public Object
  {
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Proxy(void);
    ~Proxy(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/