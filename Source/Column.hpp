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
    /* 10 */ CT_VARBINARY,
    /* 11 */ CT_VARCHAR,
    /* 12 */ CT_ENUMS
  };

  static Str* aCT[]
  {
    /*  0 */ "NONE"_Str,
    /*  1 */ "BOOLEAN"_Str,
    /*  2 */ "CHAR"_Str,
    /*  3 */ "DATE"_Str,
    /*  4 */ "DECIMAL"_Str,
    /*  5 */ "FLOAT"_Str,
    /*  6 */ "INTEGER"_Str,
    /*  7 */ "INTERVAL"_Str,
    /*  8 */ "TIME"_Str,
    /*  9 */ "TIMESTAMP"_Str,
    /* 10 */ "VARBINARY"_Str,
    /* 11 */ "VARCHAR"_Str,
    /* 12 */ "ENUMS"_Str
  };

  class Column : public Object
  {
    private:
    i8    m_iColumnNo;
    s32   m_sColumnName;
    eCT   m_eColumnType;
    i8    m_iColumnLength;
    i8    m_iColumnPrecision;
    eDM   m_eDatumType;
    bool  m_bIsNullable;
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Column(void);
    Column(Str* aName);
    Column(Str* aName, Str* aDefinition);
    ~Column(void);
  
    i8   iColumnLength(void);
    void vColumnLength(i8 iColumnLength);
  
    i8   iColumnPrecision(void);
    void vColumnPrecision(i8 iColumnPrecision);
  
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
  
    void vSet(Str* aDefinition);
    void vShow(void);
  
    bool bValidateField(Str** aaSubRow);
  
    char* aValue(i8& nBytes, char** aaCursor);
  
    bool bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    bool bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    bool bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    bool bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    bool bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    bool bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
  
    i8 iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);

    bool bCmp(eRO eRelOp, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
  
    Footer* pFooter(void);
  };
  
  Column* _pColumn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
