/*============================================================================*/
/* Slots.hpp - table partition slots                                          */
/*============================================================================*/
#pragma once


namespace MetaData
{
  class Table;
  
  /*--------------------------------------------------------------------------*/
  /* TablePtnVec -- Table Partition Vector                                    */
  /*--------------------------------------------------------------------------*/
  typedef VectorNL_t  <CN_TABLEPTNVEC,Partitioning::TablePtn,
                       C_MAX_SLOTS>                             TablePtnVec;
  
  class Slots : public Object
  {
  private:
    Table* m_pTable;   // table this slots belongs to
    Dic    m_xSubDic;  // table partition sub dictionary
    StreamDef*    m_pPtnStreamDef;   // partition stream def
    TablePtnVec*  m_pTablePtnVec;    // table partition vector
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    Slots(void);
    ~Slots(void) noexcept(false);
    
    void       vCreateColumnVec(void);
    Partitioning::TablePtn*  pPartition(i8 iSlot);
    bool       bPopulateIndex(i8 iIndexNo);
    StreamDef* pPtnStreamDef(void);
    void       vPublishTable(i8 iPartitionNo);
    bool       bPurgeTable(i8 iPartitionNo, i8 iOlderThanSeconds);
    Dic*       pSubDic(void);
    void       vTable(Table* pTable);
    void       vTidyUp(void);
    void       vTidyUpIndex(i8 iIndexNo);
    void       vTruncateIndex(i8 iPartitionNo, i8 iIndexNo);
    void       vTruncateTable(i8 iPartitionNo);
    
    Footer*    aFooter(void);
  };
  
  Slots* _pSlots(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
