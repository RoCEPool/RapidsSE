/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/
#pragma once

#include "IndexDef.hpp"

static Stz* S_READAPI_I = "__READAPI_I"_Stz;

namespace STORE
{
  class Index : public Operand
  {
  private:
    IndexDef*  m_pIndexDef;  // definition of this index

    Location   m_oLocation;   // logical location of index

    i8         m_iIndexNo;   //
    s32        m_sIndexNm;   //
  
    Table*     m_pTable;     //
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Index* pIndex);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vMonitorPortrait(s48K& sPortrait);

    Index(Stz* szIndexNm);
   ~Index(void) noexcept(false);
  
    Stz*   pAnalyze(void);
    void   vAnalyze(i8 i_TablePtn, i8& nBuffers, i8& nEntries);
    i8     nColumns(void);
    Column* pGetColumn(i8 iColumn);
    IndexDef* pIndexDef(void);
    void   vIndexDef(IndexDef* pIndexDef);
    Stz*   szIndexNm(void);
    void   vIndexNo(i8 iIndexNo);
    i8     iIndexNo(void);
    void   vListContents(i8 i_Depth);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bPrimaryKey);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
    i8     iSortKeyDatum(void);
    void   vTable(Table* pTable);
    Table* pTable(void);
    void   vTidyUp(void);
    
    static void vPostRecv(void* aIndex);
  
    Footer* aFooter(void);
  };
  
  Index* _pIndex(void* pAddr);


  typedef VectorNL_t<CN_IDXVEC,Index,C_MAX_INDEXES> IdxVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
