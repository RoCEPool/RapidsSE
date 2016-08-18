/*============================================================================*/
/* Partitions.hpp - Table Partitions                                           */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects {
    
  class Partitions
  {
  private:
    Partition m_xPartition[16];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partitions(void);
    ~Partitions(void);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/