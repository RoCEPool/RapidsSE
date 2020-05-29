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
    
    TablePtn* m_pPartition;
    //EbStrand* m_pEbStrand;
    EntryBuffer* m_pRowBuffer;
    Entry*       m_pRow;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, SelCursor* pSelCursor);
    void  operator delete(void*);
    
    SelCursor(void);
    ~SelCursor(void) noexcept(false);
    
    static void vFinalize(eCN eClass, Object* pObject);
    
    void    vBegin(i8 iPartition);
    void    vEnd(void);
    void    vFirst(void);
    void    vNext(void);
    Entry*  pRow(void);
    Stz*    aRowAsStr(void);
    EntryBuffer* pRowBuffer(void);
    Table*  pTable(void);
    void    vTable(Table* pTable);
    
    Footer* aFooter(void);
  };
  
  SelCursor* _pSelCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
