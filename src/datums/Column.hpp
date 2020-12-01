/*============================================================================*/
/* Column.hpp - Column                                                        */
/*============================================================================*/
#pragma once

namespace Datums
{
  class StreamPix // condensed stream def
  {
  public:
  
    // IF YOU ADD A MEMBER CHANGE CALC IN nBytes()
  
    i8 m_nCols;       // number of columns
    i8 m_nExtraCols;  //   of which there are nExtra metadata columns
    b8 m_bPtnHash;    // is there a partition hash ?
    i8 m_nMaxRowSize; //

    bool   m_bProject[C_MAX_DEFCOLUMNS];
    ColPix m_xaColPix[C_MAX_DEFCOLUMNS]; // col pix (C++ flexible member array must be last)
    
    // constructor
    StreamPix(void)
    {
      m_nCols       = 0;
      m_nExtraCols  = 0;
      m_bPtnHash    = false;
      m_nMaxRowSize = 256;  // FOR TESTING PURPOSES
      
      for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++) {
        m_xaColPix[i_Col].m_uAsI4 = 0;
        m_bProject[i_Col] = true;
      }
    }
  
    // calc number of bytes
    i8 nBytes(i8 nCols)
    {
      i8 nBytes = (3 * (i8)sizeof(i8)) + (nCols * ((i8)sizeof(ColPix) + sizeof(bool)));
    
      return nBytes;
    }
  
    // is column being projected
    inline i8 bIsProjected(i8 i_Col)
    {
      return m_bProject[i_Col];
    }
    
    // mark column to be not projected
    void vNotProject(i8 i_Col)
    {
      m_bProject[i_Col] = false;
    }
    
    // remove partition metadata column
    void vRemovePtnHash(void)
    {
      ASSERT(m_bPtnHash);
      ASSERT(m_nExtraCols);

      m_nCols--;
      m_nExtraCols--;
      m_bPtnHash = false;
    }
    
    // calc size of this StreamPix
    i8 nSize(void)
    {
      i8 nSize = nBytes(m_nCols);
    
      return nSize;
    }
  
    // print
    void vPrint(void)
    {
      vTrace("StreamPix: %lld cols\n"_Stz,m_nCols);
      for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
        vTrace("[%02lld] P:%s %s(%lld,%lld)%s%s\n"_Stz,
               i_Col,
               bIsProjected(i_Col) ? "true"_Stz : "false"_Stz ,
               aDM[m_xaColPix[i_Col].m_uDatum],
               (i8)m_xaColPix[i_Col].m_uLength,
               (i8)m_xaColPix[i_Col].m_uScale,
               m_xaColPix[i_Col].m_bPtnKey ? " !PtnKey" : "",
               i_Col + 1 == m_nCols && m_nExtraCols ? " #PtnHsh" : ""
               );

      }
    }
  
  };
}

/*----------------------------------------------------------------------------*/

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
