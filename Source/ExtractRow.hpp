/*============================================================================*/
/* ExtractRow.hpp -- Extract Row                                              */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class ExtractRow : public Object
  {
  public:
    RowSingle* m_pRow;         // row
    Table*     m_pTable;       // table
    Format     m_oFormatters;  // formatters
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExtractRow(Table* pTable);
   ~ExtractRow(void) noexcept(false);
    
  public:
    bool bExtractRow(RowSingle** ppRow, char** aazRecStr, bool &bBlankLine);
    
  private:
    Footer* aFooter(void);
  };
  
  ExtractRow* _pExtractRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
