/*============================================================================*/
/* Column.hpp - Column                                                        */
/*============================================================================*/
#pragma once

#include "eCT.h"
#include "eDG.h"
#include "eDM.h"
#include "eFA.h"

namespace Datums
{
  class Column : public Object
  {
    private:
    i8    m_iColumnNo;      // this columns subscript
    s32   m_sColumnNm;      // name of the column
    eDM   m_eDatum;         // SE internal datum type (DM_INT etc)

    eCT   m_eColumnType;    // column type (CT_INTEGER etc)
    i8    m_iColumnLength;  // logical length of the column
    i8    m_iColumnScale;   // logical scale of the column
  
    bool  m_bIsNullable;    // is column nullable?
    bool  m_bIsUnique;      // is column unique?
    bool  m_bIsPtnKey;      // is part of partition key?
    bool  m_bIsLastColumn;  // is last column
  
    eDG   m_eGeneration;    // data generation (DG_ASCENDING etc)
    Datum_y32 m_xMaxVal;    // max value that can be generated
    Datum_y32 m_xMinVal;    // min value that can be generated
  
    eFA   m_eFunApplied;    // function to be applied (FA_COUNT etc)
    i8    m_iFunColArg;     // which column is the arg for applied fun

    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Column(void);
   ~Column(void) noexcept(false);
  
    static eDM  eDatum(eCT eColumnType);
    static bool bValidBcd(i8 iPrecision, i8 iScale);
    static bool bValidFlt(i8 iSize);
    static bool bValidVch(i8 iSize);
    
    i8   iColumnLength(void);
    void vColumnLength(i8 iColumnLength);
  
    i8   iColumnScale(void);
    void vColumnScale(i8 iColumnScale);
  
    void vColumnNo(i8 iColumnNo);
  
    Stz* szColNm(void);
    void vColumnNm(Stz* sColumnNm);
  
    eCT  eColumnType(void);
    void vColumnType(eCT eColumnType);

    void vCopy(Column* pCol);
  
    void vDatum(eDM eDatum);
  
    eDG  eGeneration(void);
    void vGeneration(eDG eGeneration);

    bool bIsLastColumn(void);
    void vIsLastColumn(bool bIsLastColumn);
    
    bool bIsName(s32& sName);
  
    bool bIsUnique(void);
    void vIsUnique(bool bIsUnique);
    
    bool bIsNullable(void);
    void vIsNullable(bool bIsNullable);
  
    bool bIsPtnKey(void);
    void vIsPtnKey(bool bIsPtnKey);

    Datum_h* pMaxVal(void);
    Datum_h* pMinVal(void);

    void vShow(void);
  

    byte* ayValue(i8& nBytes, byte** aayCursor);
  
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
