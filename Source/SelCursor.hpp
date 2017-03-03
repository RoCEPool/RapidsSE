/*============================================================================*/
/* SelCursor.hpp - select all cursor                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class SelCursor : public Object
  {
  private:
    Table*     m_pTable;
    i8         m_iPartition;
    
    Partition* m_pPartition;
    RowStrand* m_pRowStrand;
    RowBuffer* m_pRowBuffer;
    Row*       m_pRow;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, SelCursor* pSelCursor);
    void  operator delete(void*);
    
    SelCursor(void);
    ~SelCursor(void);
    
    void    vBegin(i8 iPartition);
    void    vEnd(void);
    void    vFirst(void);
    void    vNext(void);
    Row*    pRow(void);
    char*   pRowAsStr(void);
    RowBuffer* pRowBuffer(void);
    Table*  pTable(void);
    void    vTable(Table* pTable);
    
    Footer* pFooter(void);
  };
  
  SelCursor* _pSelCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
