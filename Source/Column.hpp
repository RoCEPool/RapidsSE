/*============================================================================*/
/* Column.hpp - Column                                                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  enum eCT // Column Type
  {
    /*  0 */ CT_NONE=0,
    /*  1 */ CT_BOOLEAN,
    /*  2 */ CT_CHAR,
    /*  3 */ CT_DATE,
    /*  4 */ CT_DECIMAL,
    /*  5 */ CT_FLOAT,
    /*  6 */ CT_INTEGER,
    /*  7 */ CT_INTERVAL,
    /*  8 */ CT_TIME,
    /*  9 */ CT_TIMESTAMP,
    /* 10 */ CT_VARCHAR,
    /* 11 */ CT_ENUMS
  };

  static Stz* aCT[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "BOOLEAN"_Stz,
    /*  2 */ "CHAR"_Stz,
    /*  3 */ "DATE"_Stz,
    /*  4 */ "DECIMAL"_Stz,
    /*  5 */ "FLOAT"_Stz,
    /*  6 */ "INTEGER"_Stz,
    /*  7 */ "INTERVAL"_Stz,
    /*  8 */ "TIME"_Stz,
    /*  9 */ "TIMESTAMP"_Stz,
    /* 10 */ "VARCHAR"_Stz,
    /* 11 */ "ENUMS"_Stz
  };

  static Stz* aCT_short[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "b"_Stz, // boolean
    /*  2 */ "c"_Stz, // char
    /*  3 */ "d"_Stz, // date
    /*  4 */ "m"_Stz, // decimal
    /*  5 */ "f"_Stz, // float
    /*  6 */ "i"_Stz, // integer
    /*  7 */ "l"_Stz, // interval
    /*  8 */ "t"_Stz, // time
    /*  9 */ "p"_Stz, // timestamp
    /* 10 */ "s"_Stz, // varchar
    /* 11 */ "ENUMS"_Stz
  };

  class Column : public Object
  {
    private:
    i8    m_iColumnNo;
    s32   m_sColumnName;
    eCT   m_eColumnType;
    i8    m_iColumnLength;
    i8    m_iColumnScale;
    eDM   m_eDatumType;
    bool  m_bIsNullable;
    bool  m_bIsPartitionKey;
  
    bool  (*m_fGreaterEqual)(char* aArgA, char* aArgB);
    bool  (*m_fGreaterThan)(char* aArgA, char* aArgB);
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Column(void);
    Column(Stz* aName);
    Column(Stz* aName, Stz* aDefinition);
    Column(JobModel::Job_CreateTable* pJob, i8 iColumnNo);
    ~Column(void) noexcept(false);
  
    i8   iColumnLength(void);
    void vColumnLength(i8 iColumnLength);
  
    i8   iColumnScale(void);
    void vColumnScale(i8 iColumnScale);
  
    i8   iColumnNo(void);
    void vColumnNo(i8 iColumnNo);
  
    s32& sColumnName(void);
    void vColumnName(s32& sColumnName);
  
    eCT  eColumnType(void);
    void vColumnType(eCT eColumnType);
  
    eDM  eDatumType(void);
    void vDatumType(eDM eDatumType);
  
    bool bIsName(s32 sName);
  
    bool bIsNullable(void);
    void vIsNullable(bool bIsNullable);
  
    bool bIsPartitionKey(void);
    void vIsPartitionKey(bool bIsNullable);

    void vSet(Stz* aDefinition);
    void vShow(void);
    
    char* aValue(i8& nBytes, char** aaCursor);
  
    bool bLessThan(char* aArgA, char* aArgB);
    bool bLessEqual(char* aArgA, char* aArgB);
    bool bEqualTo(char* aArgA, char* aArgB);
    bool bNotEqual(char* aArgA, char* aArgB);
    bool bGreaterEqual(char* aArgA, char* aArgB);
    bool bGreaterThan(char* aArgA, char* aArgB);
  
    i8 iCmp(char* aArgA, char* aArgB);

    bool bCmp(eRO eRelOp, char* aArgA, char* aArgB);
  
    Footer* aFooter(void);
  };
  
  Column* _pColumn(void* aAddr);
  
  /*--------------------------------------------------------------------------*/
  /* ColumnVec -- Column Vector                                               */
  /*--------------------------------------------------------------------------*/
  typedef Vector<CN_COLUMNVEC,Column,CN_COLUMN,1024,u8> ColumnVec;
  
  /*--------------------------------------------------------------------------*/
  /* IdxColVec -- Index Column Vector                                         */
  /*--------------------------------------------------------------------------*/
  typedef Vector<CN_IDXCOLVEC,Column,CN_COLUMN,1024,u8> IdxColVec;
  
  /*--------------------------------------------------------------------------*/
  /* PtnColVec -- Partition Column Vector                                     */
  /*--------------------------------------------------------------------------*/
  typedef Vector<CN_PTNCOLVEC,Column,CN_COLUMN,1024,u8> PtnColVec;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
