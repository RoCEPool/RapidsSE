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
    s256   m_sIndexNm;       // name of index
    s256   m_sTableNm;       // name of table
    eIT    m_eIndexType;     // index type
    eKE    m_eKeyKind;       // key entry kind
  
    Location   m_oLocation;   // logical location of indexDef

    bool   m_bIsPrimaryKey;  // is this the primary key?
    bool   m_bIsUnique;      // is this index unique?
    bool   m_bIsGlobal;      // is this index global?
    bool   m_bHasDescending; // has a desc column

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
    static eEC eParseTableName(Stz* szCmd, String* psTableNm);
    static void vPostRecv(void* aIndexDef);

    void    vAddColumn(Stz* szColumnNm, TypeId xTypeId, bool bAscending=true);

    i8      nColumns(void);
    i8*     aiaColSet(void);
    void    vDetermineKeyKind(void);
    void    vDump(i8 i_Tab=0);
    Column* pFindColumn(Stz* szColumnNm);
    Column* pFindColumn(i8 i_ColSet);
    Column* pGetColumn(i8 i_Column);
    i8     iGetColumn(i8 iColSet);
    i8     iGetStreamDefCol(i8 iColumn);
    bool   bIsGlobal(void);
    void   vIsGlobal(bool bGlobal);
    bool   bHasDescending(void);
    void   vHasDescending(bool bHasDescending);

    Stz*   szIndexNm(void);
    void   vIndexNm(Stz* szIndexNm);
    eIT    eIndexType(void);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bUnique);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
    eKE    eKeyKind(void);
    void   vKeyKind(eKE eKeyKind);
    void   vListContents(i8 i_Tab);
    Location* pLocation(void);
    void   vPrint(void);
    void   vSetColSet(i8 iColumnNo, i8 iColumnSet);
    Stz*   szTableNm(void);
    void   vTableNm(Stz* szTableNm);
    eEC    eParse(Table* pTable, Stz* szCmd);
    void   vResetPointers(void);
    i8     iSortKeyDatum(void);

    void   vValidate(void);
  
  private:
    eEC eAddIdxCol(Table* pTable, i8 iCol, Stz* szColNm);
  
    Footer* aFooter(void);
  };
  
  IndexDef* _pIndexDef(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
