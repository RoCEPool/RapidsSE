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
  
  static Stz* aIT[]
  {
    /* 0 */ "NONE"_Stz,
    /* 1 */ "SKIP_LIST"_Stz
  };

  class Table;
  class Partition;
  
  class Index : public Object
  {
  private:
    i8           m_iIndexNo;      //
    s32          m_sIndexName;    //
    eIT          m_eIndexType;    //
    bool         m_bIsPrimaryKey; //
    bool         m_bIsUnique;     //
    Table*       m_pTable;        //
    Dic          m_xSubDic;       // sub dictionary for this index
    IdxColVec*   m_pIdxColVec;    //
    bool         m_bIsHidden;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Index* pIndex);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    Index(void);
    ~Index(void);
  
    Stz*   pAnalyze(void);
    void   vCreateColumnVec(s32& sName, s48K& sDefinition);
    void   vDump(void);
    Stz*   aIndexName(void);
    s32    sIndexName(void);
    void   vIndexNo(i8 iIndexNo);
    i8     iIndexNo(void);
    bool   bIsHidden(void);
    void   vIsHidden(bool bHidden);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bUnique);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
    void   vShowColumnVec(void);
    Dic*   pSubDic(void);
    void   vTable(Table* pTable);
    Table* pTable(void);
    void   vTidyUp(void);
    
    IdxColVec* pIdxColVec(void);
  
    Footer* aFooter(void);
  };
  
  Index* _pIndex(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
