/*============================================================================*/
/* Slots.hpp - table partition slots                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  // allow for maximum 8 partitions per node
  static const i8 C_SLOTS_PER_NODE = 64;
  
  class Slots : public Object
  {
  private:
    Table*     m_pTable;         // table this slots belongs to
    Dic        m_xSubDic;        // partitionn sub dictionary
    PtnColVec* m_pPtnColVec;     // partition colummn vector
    Partition  m_xPartition[C_SLOTS_PER_NODE];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    Slots(void);
    ~Slots(void);
    
    void       vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn);
    Partition* pPartition(i8 iSlot);
    void       vPopulateIndex(i8 iIndexNo);
    PtnColVec* pPtnColVec(void);
    void       vShowColumnVec(void);
    Dic*       pSubDic(void);
    void       vTable(Table* pTable);
    void       vTidyUp(void);
    void       vTidyUpIndex(i8 iIndexNo);
    void       vTruncate(void);
    void       vTruncateIndex(i8 iIndexNo);
    
    Footer*    aFooter(void);
  };
  
  Slots* _pSlots(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
