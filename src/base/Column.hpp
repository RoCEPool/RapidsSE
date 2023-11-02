/*============================================================================*/
/* Column.hpp -- Column                                                       */
/*============================================================================*/
#pragma once

#include "eCT.h"
#include "eDM.h"
#include "TypeId.hpp"

namespace STORE { class InfoSet; }

namespace BASE
{
  class Column : public Object
  {
    private:
    i8    m_iColumnNo;      // this columns subscript
    s32   m_sColumnNm;      // name of the column
    eCT   m_eColumnType;    // column type (CT_INTEGER etc)
		Parser::TypeId m_xColumnTypeId; // column type id

    bool  m_bIsUnique;      // is column unique?
    bool  m_bIsLastColumn;  // is last column

    eDM   m_eDatum;         // SE internal datum type (DM_INT etc)
    i8    m_iColumnLength;  // logical length of the column
    i8    m_iColumnScale;   // logical scale of the column
    bool  m_bIsNullable;    // is column nullable?
    bool  m_bIsPtnKey;      // is part of partition key?

    bool m_bIsAscending;    // is this column in ascending order ?
    
    ColPix m_xColPix;
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Column(void);
   ~Column(void) noexcept(false);
  
    static eDM  eDatum(eCT eColumnType);
    static eCT  eColumn(Stz* szType);
    static eCT  eColumnType(eDM eDatum);
    static bool bValidBcd(i8 iPrecision, i8 iScale);
    static bool bValidFlt(i8 iSize);
    static bool bValidVch(i8 iSize);

    void vIsAscending(bool bIsAscending);
    bool bIsAscending(void);

    i8   iColumnLength(void);
    void vColumnLength(i8 iColumnLength);
  
    i8   iColumnScale(void);
    void vColumnScale(i8 iColumnScale);
  
    void vColumnNo(i8 iColumnNo);
  
    Stz* szColNm(void);
    void vColumnNm(Stz* sColumnNm);
    i8   iColNmBufSize(void);
  
    eCT  eColumnType(void);
    void vColumnType(eCT eColumnType);

		Parser::TypeId xColumnTypeId(void);
		void vColumnTypeId(Parser::TypeId xTypeId);

    void vCopy(Column* pCol);
    void vCopy(Column* pCol, Stz* sColPrefix);
  
    void vDatum(eDM eDatum);
  
    bool bIsLastColumn(void);
    void vIsLastColumn(bool bIsLastColumn);
    
    bool bIsName(s32& sName);
  
    bool bIsUnique(void);
    void vIsUnique(bool bIsUnique);
    
    bool bIsNullable(void);
    void vIsNullable(bool bIsNullable);
  
    bool bIsPtnKey(void);
    void vIsPtnKey(bool bIsPtnKey);
  
    void vListContents(i8 i_Depth);
  
    void vSet(Stz* sColumnNm, eCT eColumnType, i8 iColumnLength, i8 iColumnScale);
    void vShow(void);
  
    char cTypeShort(void);
    
    Footer* aFooter(void);
  
    /*----------------------------------------------------------------------------*/
    /* get column no                                                              */
    /*----------------------------------------------------------------------------*/
    inline i8 iColumnNo(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
    
      return m_iColumnNo;
    }
  
    /*----------------------------------------------------------------------------*/
    /* get datum type                                                             */
    /*----------------------------------------------------------------------------*/
    inline eDM eDatum(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
    
      return m_eDatum;
    }
  
  };
  
  Column* _pColumn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
