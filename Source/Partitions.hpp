/*============================================================================*/
/* Partitions.hpp - table partitions                                           */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  static const i8 C_MAX_PARTITIONS_PER_NODE = 8;
  
  class Partitions : public Object
  {
  private:
    Partition m_xPartition[C_MAX_PARTITIONS_PER_NODE];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partitions(void);
    ~Partitions(void);
    
    Partition* pPartition(i8 iPartition);
    
    void vTruncate(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
