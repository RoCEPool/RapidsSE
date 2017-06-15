/*============================================================================*/
/* Partition.hpp - table partition                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  class EntWrk;
  
  // allow for maximum 8 indexes per table
  static const i8 C_INDEXES_PER_TABLE = 8;

  typedef Vector<CN_IDXDATAVEC,IndexData,CN_INDEXDATA,C_INDEXES_PER_TABLE,u8> IdxDataVec;
  
  class Partition : public Object
  {
  private:
    i8           m_iPartitionId;   // partition number
    Data*        m_pData;
    IdxDataVec*  m_pIdxDataVec;
  
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
  
    void    vAddIndex(IndexData* pIndexData);
    
    bool    bInsertRow(EntryBuffer* pRowBuffer, EntWrk& xRowWrk);
    i8      iPartitionId(void);
    void    vPartitionId(i8 iPartitionId);
    void    vPopulateIndex(i8 iIndexNo);
    Data*   pData(void);
    
    IndexData*  pIndexData(i8 iIndexNo);
    IndexData*  pHiddenIndexData(void);
    IdxDataVec* pIdxDataVec(void);
    void        vTidyUp(void);
    void        vTidyUpIndex(i8 iIndexNo);
    
  private:
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
