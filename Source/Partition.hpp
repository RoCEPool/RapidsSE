/*============================================================================*/
/* Partition.hpp - table partition                                            */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Partition
  {
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partition(void);
    ~Partition(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/