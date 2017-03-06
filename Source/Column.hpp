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

  class Column
  {
    private:
    i8    m_iColumnNo;
    s32   m_sColumnName;
    eCT   m_eColumnType;
    bool  m_bIsNullable;
    i8    m_nColumnMgr;
    
    // must be last
    Footer m_xFooter;
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
