/*============================================================================*/
/* Partitions.hpp - Table Partitions                                           */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects {
    
  class Partitions : public Object
  {
    
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/