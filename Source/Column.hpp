/*============================================================================*/
/* Column.hpp - Column                                                        */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Column;
  
  /*--------------------------------------------------------------------------*/
  /* ColumnVec -- Column Vector                                               */
  /*--------------------------------------------------------------------------*/
  typedef VectorNL_t<CN_COLUMNVEC,Column,1024> ColumnVec;
  
  /*--------------------------------------------------------------------------*/
  /* IdxColVec -- Index Column Vector                                         */
  /*--------------------------------------------------------------------------*/
  typedef VectorNL_t<CN_IDXCOLVEC,Column,1024> IdxColVec;
  
  /*--------------------------------------------------------------------------*/
  /* PtnColVec -- Partition Column Vector                                     */
  /*--------------------------------------------------------------------------*/
  typedef VectorNL_t<CN_PTNCOLVEC,Column,1024> PtnColVec;
  
  
  class Column : public Object
  {
    private:
    i8    m_iColumnNo;
    s32   m_sColumnNm;
  //s32   m_sAlias;
    eCT   m_eColumnType;
    i8    m_iColumnLength;
    i8    m_iColumnScale;
    eDM   m_eDatumType;
    bool  m_bIsNullable;
    bool  m_bIsUnique;
    bool  m_bIsPtnKey;
    bool  m_bIsLastColumn;
  
    ColumnVec* m_pColVec;
    
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Column(void);
    //Column(Jobs::Job_CreateTable* pJob, i8 iColumnNo);
    ~Column(void) noexcept(false);
  
    static bool bValidBcd(i8 iPrecision, i8 iScale);
    static bool bValidFlt(i8 iSize);
    static bool bValidVch(i8 iSize);
  
    i8   iColumnLength(void);
    void vColumnLength(i8 iColumnLength);
  
    i8   iColumnScale(void);
    void vColumnScale(i8 iColumnScale);
  
    //i8   iColumnNo(void);
    void vColumnNo(i8 iColumnNo);
  
    Stz* szColNm(void);
    void vColumnNm(Stz* sColumnNm);
  
    eCT  eColumnType(void);
    void vColumnType(eCT eColumnType);

    //ColumnVec* pColVec(void);
    void vColVec(ColumnVec* pColVec);

    void vCopy(Column* pCol);
  
    //eDM  eDatumType(void);
    void vDatumType(eDM eDatumType);
  
    bool bIsLastColumn(void);
    void vIsLastColumn(bool bIsLastColumn);
    
    bool bIsName(s32& sName);
  
    bool bIsUnique(void);
    void vIsUnique(bool bIsUnique);
    
    bool bIsNullable(void);
    void vIsNullable(bool bIsNullable);
  
    bool bIsPtnKey(void);
    void vIsPtnKey(bool bIsPtnKey);

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
    inline eDM eDatumType(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
    
      return m_eDatumType;
    }
  
    /*----------------------------------------------------------------------------*/
    /* get column vector                                                          */
    /*----------------------------------------------------------------------------*/
    inline ColumnVec* pColVec(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMN,_FOOTER_);
    
      return m_pColVec;
    }
  };
  
  Column* _pColumn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
