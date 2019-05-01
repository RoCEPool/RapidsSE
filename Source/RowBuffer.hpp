/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowCoder;
  
  class RowBuffer : public Object
  {
  friend class IndexTree;

  protected:
    Lock    m_okLock;               // lock
    i8      m_iBufferNo;            // this RowBuffer's buffer number
    i8      m_iPartitionNo;         // partition no this buffer belongs to
    RowTray m_xRowTray;             // tray of rows
  
    RowBuffer* m_pNextBuffer;       // next buffer
    RowBuffer* m_pPrevBuffer;       // previous buffer
    
    Since  m_oWhenNewed;            // when was this buffer created?
  
    SeeMe m_xSeeMe;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    RowBuffer(void);
    ~RowBuffer(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static RowBuffer* pRefdRowBuffer(Id& xId);
    static RowBuffer* pToRowBuffer(void* aAddr);
    static bool bCheckAddress(RowBuffer* pRowBuffer, void* aAddr);
    
    void    vDropWrite(void);
    void    vGrabWrite(void);
    void    vTable(Table* pTable);
    Table*  pTable(void);
    i8      nRowCount(void);

    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
    i8          iAgeSsec(void);
    void        vBufferCount(i8& nBufferCount);
    i8          iDecile(void);
    i8          iLengthList(void);
    RowBuffer*  pNextBuffer(void);
    void        vNextBuffer(RowBuffer* pNextBuffer);
    void        vPartitionNo(i8 iPartitionNo);
    i8          iPartitionNo(void);
    RowBuffer*  pPrevBuffer(void);
    void        vPrevBuffer(RowBuffer* pPrevBuffer);
    RowTray*    pRowTray(void);
    void        vTruncate(void);
    Since&      oWhenNewed(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // row functions
    void  vShow(void);
    void  vShowRow(i8 iRowNo);
    HeadLine* pRow(i8 iRow);

    ////////////////////////////////////////////////////////////////////////////
    // row functions
    bool bAppendRow(RowCoder* pRowCoder, RowNub* pRowNub);
  
  private:
    HeadLine* aAllotRow(i8 nBytes);
    HeadLine* aAllotHead(void);
    bool       bHasSpace(i8 nBytes);

    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertRowBuffer(void);
    bool bValidRowBuffer(void);
  
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iRows);
    void        vDump(void);
    void        vRestore(void);
    void        vRestoreTable(Table* pTable);
    bool        bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions        
    bool         bVerifyNextPrev(void);
    bool         bVerifyRows(void);
    void         vVerifyFault(void);

    public:
    Footer* aFooter(void);

  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
