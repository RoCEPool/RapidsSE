/*============================================================================*/
/* Object.hpp - root class for objects in buffer pool                         */
/*============================================================================*/

#pragma once

#include "Pool.h"

namespace Pool
{
  class Object : public mn,
                 public cn
  {
  private:
    
  public:
    Object(eCN eClass);
    
    void vInitMagic(void);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
