/*============================================================================*/
/* IndexDef.hpp -- Index Definition                                           */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class IndexDef : public Definition
  {
  friend class Index;
  
  private:
    s32    m_sIndexNm;       // name of index
    s32    m_sTableNm;       // name of table
    eIT    m_eIndexType;     // index type
  
    bool   m_bIsPrimaryKey;  // is this the primary key?
    bool   m_bIsUnique;      // is this index unique?
  
    IdxColVec* m_pIdxColVec; // the sub-set of columns which are this index

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexDef(void);
   ~IndexDef(void) noexcept(false);
  
    static eEC eSyntax(Stz* szCmd);
    
    Stz*   szIndexNm(void);
    eEC    eParse(Table* pTable, Stz* szCmd);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bUnique);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
    i8     iSortKeyDatum(void);
  
    IdxColVec* pIdxColVec(void);
  
  private:
    bool bAddIdxCol(Table* pTable, Stz* szColNm);
  
    Footer* aFooter(void);
  };
  
  IndexDef* _pIndexDef(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
