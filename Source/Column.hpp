/*============================================================================*/
/* Column.hpp - Column                                                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  enum eCT // Column Type
  {
    /* 0 */ CT_NONE=0,
    /* 1 */ CT_XYZ
  };

  static Str* aCT[]
  {
    /* 0 */ "NONE"_Str,
    /* 1 */ "XYZ"_Str
  };

  class Column : public Object
  {
    private:
    i8    m_iColumnNo;
    s32   m_sColumnName;
    eCT   m_eColumnType;
    eDM   m_eDatumType;
    bool  m_bIsNullable;
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Column(void);
    Column(Str* aName, Str* aDefinition);
    ~Column(void);
  
    i8   iColumnNo(void);
    void vColumnNo(i8 iColumnNo);
  
    s32& sColumnName(void);
    void vColumnName(s32& sColumnName);
  
    eCT  eColumnType(void);
    void vColumnType(eCT eColumnType);
  
    eDM  eDatumType(void);
    void vDatumType(eDM eDatumType);
  
    bool bIsNullable(void);
    void vIsNullable(bool bIsNullable);
  
    void vSet(Str* aDefinition);
  
    Footer* pFooter(void);
  };
  
  Column* _pColumn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
