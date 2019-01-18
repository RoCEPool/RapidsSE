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

  enum eIF // Index Form
  {
    /* 0 */ IF_NONE=0,
    /* 1 */ IF_I8,
    /* 2 */ IF_I8_I8,
    /*   */ IF_I8_I8_I8,
    /* 3 */ IF_STZ,
    /* 4 */ IF_COMPLEX
  };

  static Stz* aIF[]
  {
  /* 0 */ "NONE"_Stz,
  /* 1 */ "I8"_Stz,
  /* 2 */ "I8_I8"_Stz,
  /* 3 */ "STZ"_Stz,
  /* 4 */ "COMPLEX"_Stz,
  };
  
  class Table;
  class TablePtn;
  
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
  
    bool         m_bSingleColumn; // is this index only over a signle column
    bool         (*m_fGreaterEqual)(char* aArgA, char* aArgB);
    bool         (*m_fGreaterThan)(char* aArgA, char* aArgB);
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Index* pIndex);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    Index(void);
    ~Index(void) noexcept(false);
  
    Stz*   pAnalyze(void);
    void   vCreateColumnVec(s32& sName, s48K& sDefinition);
    void   vCreateColumnVec(Jobs::Job_CreateIndex* pJob);
    void   vDump(void);
  
    bool   bGreaterEqual(char* aDatumA, char* aDatumB);
    bool   bGreaterThan(char* aDatumA, char* aDatumB);
    
    eIF    eIndexForm(void);
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
    bool   bSingleColumn(void);
    void   vSetSingleColumn(void);
    void   vShowColumnVec(void);
    i8     iSortKeyDatum(void);
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
