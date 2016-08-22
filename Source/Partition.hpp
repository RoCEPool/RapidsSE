/*============================================================================*/
/* Partition.hpp - table partition                                            */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Partition : public Object
  {
  private:
    Lock       m_zLock;        // lock covers all buffers
    i8         m_iRowStrands;  // number of row strands
    RowStrand* m_pHeadStrand;  // first strand in list
    RowStrand* m_pTailStrand;  // last strand in list
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partition(void);
    ~Partition(void);
    
    bool bInsertRow(StrVal* pRowStr);
    
    RowStrand* pHeadStrand(void);
    RowStrand* pTailStrand(void);
    
  private:
    bool bExtendPartition(void);
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/