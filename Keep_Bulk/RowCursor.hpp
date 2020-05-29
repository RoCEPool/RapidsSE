/*============================================================================*/
/* RowCursor.hpp - row cursor                                                 */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class RowCursor : public Object
  {
  private:
    Table*     m_pTable;
    i8         m_iPartition;
    
    TablePtn*  m_pPartition;
    //EbStrand* m_pEbStrand;
    EntryBuffer* m_pRowBuffer;
    Entry*       m_pRow;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowCursor* pRowCursor);
    void  operator delete(void*);
    
    RowCursor(void);
    ~RowCursor(void) noexcept(false);
    
    void    vBegin(i8 iPartition);
    void    vEnd(void);
    void    vFirst(void);
    void    vNext(void);
    Entry*  pRow(void);
    Stz*    aRowAsStr(void);
    EntryBuffer* pRowBuffer(void);
    void    vRowBuffer(EntryBuffer* pRowBuffer);
    Table*  pTable(void);
    void    vTable(Table* pTable);
    
    Footer* aFooter(void);
  };
  
  RowCursor* _pRowCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
