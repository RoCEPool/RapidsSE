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
    i8         m_iPartitionId;   // partition number
    Lock       m_zLock;          // lock covers all buffers
    i8         m_iRowStrands;    // number of row strands
    RowStrand* m_pHeadStrand;    // first strand in list
    RowStrand* m_pTailStrand;    // last strand in list
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partition(void);
    ~Partition(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    static i8   iPartitionToNode(i8 iPartition);
    static i8   iPartitionToSlot(i8 iPartition);
    static i8   iSlotToPartition(i8 iSlot);

    void vAppendBuffer(RowBuffer* pRowBuffer);
    bool bInsertRow(TxRef* pTxRef, char* pRecord, i8 iLength);
    
    RowStrand* pHeadStrand(void);
    i8         iPartitionId(void);
    void       vPartitionId(i8 iPartitionId);
    RowStrand* pTailStrand(void);
    
    void vTruncate(void);
    
  private:
    bool bExtendPartition(void);
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
