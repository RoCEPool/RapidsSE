/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Index : public Object
  {
  private:
    IndexDef*  m_pIndexDef;  // definition of this index

    i8         m_iIndexNo;   //
    s32        m_sIndexNm;   //
  
    Table*     m_pTable;     //
    Dic        m_xSubDic;    // sub dictionary for this index
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Index* pIndex);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    static void vMonitorPortrait(s48K& sPortrait);

    Index(void);
   ~Index(void) noexcept(false);
  
    Stz*   pAnalyze(void);
    void   vCreateColumnVec(s32& sName, s48K& sDefinition);
    void   vCreateColumnVec(Jobs::Job_CreateIndex* pJob);
    IndexDef* pIndexDef(void);
    void   vIndexDef(IndexDef* pIndexDef);
    Stz*   szIndexNm(void);
    void   vIndexNo(i8 iIndexNo);
    i8     iIndexNo(void);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bPrimaryKey);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
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
