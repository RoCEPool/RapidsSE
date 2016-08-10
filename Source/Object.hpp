/*============================================================================*/
/* Object.hpp - root class for objects in buffer pool                         */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

namespace MemoryManagement
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
