/*============================================================================*/
/* ExtractRow.hpp -- Extract Row                                              */
/*============================================================================*/
#pragma once

namespace Buffers
{
  class RowCompactor;
  
  class ExtractRow : Object
  {
  public:
    Row*    m_pRow;         // row
    Table*  m_pTable;       // table
    Format  m_oFormatters;  // formatters
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExtractRow(void);
   ~ExtractRow(void) noexcept(false);
    
  public:
    bool bExtractRow(char** aazRecStr, bool &bBlankLine);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
