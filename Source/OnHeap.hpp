/*============================================================================*/
/* OnHeap.hpp - root class for objects on malloc heap                         */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  class OnHeap : public mn,
                 public cn
  {
  public:
    OnHeap(cn eClass);
    ~OnHeap(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
