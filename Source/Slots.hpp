/*============================================================================*/
/* Slots.hpp - table partition slots                                          */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Table;
  class TablePtn;
  
  /*--------------------------------------------------------------------------*/
  /* TablePtnVec -- Table Partition Vector                                    */
  /*--------------------------------------------------------------------------*/
  typedef VectorNL_t<CN_TABLEPTNVEC,TablePtn,CN_TABLEPTN,C_SLOTS_PER_NODE,u8> TablePtnVec;
  
  class Slots : public Object
  {
  private:
    Table* m_pTable;   // table this slots belongs to
    Dic    m_xSubDic;  // table partition sub dictionary
    PtnColVec*   m_pPtnColVec;   // partition colummn vector
    TablePtnVec* m_pTablePtnVec; // table partition vector
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    Slots(void);
    ~Slots(void) noexcept(false);
    
    void       vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn);
    void       vCreateColumnVec(void);
    TablePtn*  pPartition(i8 iSlot);
    bool       bPopulateIndex(i8 iIndexNo);
    PtnColVec* pPtnColVec(void);
    void       vPublishIndex(i8 iPartitionNo, i8 iIndexNo);
    bool       bPurgeIndex(i8 iPartitionNo, i8 iIndexNo, i8 iOlderThanSeconds);
    void       vShowColumnVec(void);
    Dic*       pSubDic(void);
    void       vTable(Table* pTable);
    void       vTidyUp(void);
    void       vTidyUpIndex(i8 iIndexNo);
    void       vTruncate(void);
    void       vTruncateIndex(i8 iPartitionNo, i8 iIndexNo);
    void       vTruncateTable(i8 iPartitionNo);
    
    Footer*    aFooter(void);
  };
  
  Slots* _pSlots(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
