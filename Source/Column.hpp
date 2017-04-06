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
    bool  m_bIsNullable;
    i8    m_nColumnMgr;
    
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Column(void);
    ~Column(void);
  
    Footer* pFooter(void);
  };
  
  Column* _pColumn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
