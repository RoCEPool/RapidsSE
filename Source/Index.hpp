/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  enum eIT // Index Type
  {
    /* 0 */ IT_NONE=0,
    /* 1 */ IT_SKIP_LIST
  };
  
  static Str* aIT[]
  {
    /* 0 */ "NONE"_Str,
    /* 1 */ "SKIP_LIST"_Str
  };

  class Table;
  class Partition;
  
  class Index : public Object
  {
  private:
    i8           m_iIndexNo;
    s32          m_sIndexName;
    eIT          m_eIndexType;
    Table*       m_pTable;
    Dic          m_xSubDic;    // sub dictionary for this index
    IdxColVec*   m_pIdxColVec;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Index* pIndex);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    Index(void);
    ~Index(void);
  
    Str* pAnalyze(void);
    void vCreateColumnVec(s32& sName, s48K& sDefinition);
    void vDump(void);
    Str* aIndexName(void);
    void vIndexNo(i8 iIndexNo);
    i8   iIndexNo(void);
    void vShowColumnVec(void);
    Dic* pSubDic(void);
    void vTable(Table* pTable);
    Table* pTable(void);
    void vTidyUp(void);
    
    IdxColVec* pIdxColVec(void);
  
    Footer* pFooter(void);
  };
  
  Index* _pIndex(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
