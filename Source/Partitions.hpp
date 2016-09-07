/*============================================================================*/
/* Partitions.hpp - table partitions                                           */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  static const i8 C_TABLE_PARTITIONS = 8;
  
  class Partitions
  {
  private:
    Partition m_xPartition[C_TABLE_PARTITIONS];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partitions(void);
    ~Partitions(void);
    
    Partition* pPartition(i8 iPartition);
    
    void vTruncate(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/