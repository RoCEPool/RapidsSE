/*============================================================================*/
/* RbCursor.hpp - row buffer cursor                                           */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class RbCursor : public Object
  {
  private:
    Table*     m_pTable;
    i8         m_iPartition;
    
    TablePtn*  m_pPartition;
    //EbStrand* m_pEbStrand;
    EntryBuffer* m_pRowBuffer;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, RbCursor* pRbCursor);
    void  operator delete(void*);
    
    RbCursor(void);
    ~RbCursor(void) noexcept(false);
    
    void    vBegin(i8 iPartition);
    void    vEnd(void);
    void    vFirst(void);
    void    vNext(void);
    EntryBuffer* pRowBuffer(void);
    Table*  pTable(void);
    void    vTable(Table* pTable);
    
    Footer* aFooter(void);
  };
  
  RbCursor* _pRbCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
