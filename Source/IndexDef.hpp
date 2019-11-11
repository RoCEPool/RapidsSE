/*============================================================================*/
/* IndexDef.hpp -- Index Definition                                           */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Table;
  
  class IndexDef : public Object
  {
  private:
    s32  m_sIndexName;    //
    eIT  m_eIndexType;    //
  
    bool m_bIsPrimaryKey; //
    bool m_bIsUnique;     //
  
    IdxColVec*   m_pIdxColVec;    //
  
    s4K m_sCmd;  // the command which created this def

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexDef(void);
   ~IndexDef(void) noexcept(false);
  
    Stz*   pAnalyze(void);
//    void   vCreateColumnVec(s32& sName, s48K& sDefinition);
//    void   vCreateColumnVec(Jobs::Job_CreateIndex* pJob);
    Stz*   aIndexName(void);
    s32    sIndexName(void);
//    void   vIndexNo(i8 iIndexNo);
//    i8     iIndexNo(void);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bUnique);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
    void   vShowColumnVec(void);
    i8     iSortKeyDatum(void);
//    void   vTable(Table* pTable);
//    Table* pTable(void);
//    void   vTidyUp(void);
  
    IdxColVec* pIdxColVec(void);
  
    Footer* aFooter(void);
  };
  
  IndexDef* _pIndexDef(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
