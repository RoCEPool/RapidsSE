/*============================================================================*/
/* Partition.hpp - table partition                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  
  // allow for maximum 8 indexes per table
  static const i8 C_INDEXES_PER_TABLE = 8;
  
  class Partition : public Object
  {
  private:
    i8         m_iPartitionId;   // partition number
    Data*      m_pData;
    Index*     m_pIndex[C_INDEXES_PER_TABLE];
  
    // must be last
    Footer m_xFooter;
    
  public:
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    static i8   iPartitionToNode(i8 iPartition);
    static i8   iPartitionToSlot(i8 iPartition);
    static i8   iSlotToPartition(i8 iSlot);
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    Partition(void);
    ~Partition(void);
  
    i8    iPartitionId(void);
    void  vPartitionId(i8 iPartitionId);
  
    Data*  pData(void);
    Index* pIndex(i8 iIndex);
  
  private:
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
