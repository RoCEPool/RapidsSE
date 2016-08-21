/*============================================================================*/
/* Partitions.hpp - table partitions                                           */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Partitions
  {
  private:
    Partition m_xPartition[1];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partitions(void);
    ~Partitions(void);
    
    Partition* pPartition(i8 iPartition);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/