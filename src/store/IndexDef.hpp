/*============================================================================*/
/* IndexDef.hpp -- Index Definition                                           */
/*============================================================================*/
#pragma once

#include "eIT.h"
#include "eKE.h"

#include "Definition.hpp"

namespace STORE
{
  class IndexDef : public Definition
  {
  friend class Index;
  
  private:
    s32    m_sIndexNm;       // name of index
    s32    m_sTableNm;       // name of table
    eIT    m_eIndexType;     // index type
    eKE    m_eKeyKind;       // key entry kind
  
    Location   m_oLocation;   // logical location of indexDef

    bool   m_bIsPrimaryKey;  // is this the primary key?
    bool   m_bIsUnique;      // is this index unique?
    
    i8     m_nColumns;       // no of columns in the index
    Column m_xaColumn[C_MAX_INDEX_COLUMNS];
    i8     m_iaColSet[C_MAX_INDEX_COLUMNS+1];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexDef(void);
    IndexDef(IndexDef* pIndexDef);
   ~IndexDef(void) noexcept(false);

    static eEC eSyntax(Stz* szCmd);
    static eEC eParseTableName(Stz* szCmd, Stz* szTableNm);
    static void vPostRecv(void* aIndexDef);

    void    vAddColumn(Stz* szColumnNm, eCT eColumnType);
    i8      nColumns(void);
    i8*     aiaColSet(void);
    void    vDetermineKeyKind(void);
    Column* pFindColumn(Stz* szColumnNm);
    Column* pGetColumn(i8 i_Column);
    Stz*   szIndexNm(void);
    void   vIndexNm(Stz* szIndexNm);
    eIT    eIndexType(void);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bUnique);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
    eKE    eKeyKind(void);
    void   vKeyKind(eKE eKeyKind);
    void   vListContents(i8 i_Depth);
    Location* pLocation(void);
    Stz*   szTableNm(void);
    void   vTableNm(Stz* szTableNm);
    eEC    eParse(Table* pTable, Stz* szCmd);
    void   vResetPointers(void);
    i8     iSortKeyDatum(void);
  
  private:
    bool bAddIdxCol(Table* pTable, i8 iCol, Stz* szColNm);
  
    Footer* aFooter(void);
  };
  
  IndexDef* _pIndexDef(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
