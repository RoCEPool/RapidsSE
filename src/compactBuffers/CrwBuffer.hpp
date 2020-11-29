/*============================================================================*/
/* CrwBuffer.hpp -- Compact Row Buffer                                        */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class CrwBuffer : public DataBuffer
  {
  public:
    RowTray    m_oRowTray;  // tray of rows
    Table*     m_pTable;
    StreamDef* m_pStreamDef;
    SeeMe      m_xSeeMe;
  
    RowCompactor* m_pRowCompactor;
  
  private:
  
    tXN m_tTxnNo; // transaction number
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    CrwBuffer(void);
    ~CrwBuffer(void) noexcept(false);

    static bool       bDeleteRefdCrw(Rrf xRef);
    static void       vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Crw*       pRefdCrw(Rrf xRef);
    static CrwBuffer* pRefdCrwBuffer(Rrf& xRef);
    static CrwBuffer* pToCrwBuffer(void* aAddr);
    static void       vToRow(RowSingle* pRow, Crw* aCrw);

    void       vDelete(Crw* aCrw);
    HeadLine*  aGetHead(i8 i_Row);
    void       vOperandDump(i8 iDetail);
    i8         nRowCount(void);
    void       vStreamDef(StreamDef* pStreamDef);
    void       vTable(Table* pTable);
    Table*     pTable(void);

    tXN        tTxnNo(void);
    void       vTxnNo(tXN tTxnNo);

    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
    void       vBufferCount(i8& nBufferCount);
    void       vCopy(CrwBuffer* pArgCrwBuffer);
    i8         iDecile(void);
    i8         iLengthList(void);
    CrwBuffer* pNextCrwBuffer(void);
    CrwBuffer* pNextCrwBuffer(PtnStream* pPtnStream);
    void       vNextCrwBuffer(CrwBuffer* pNextBuffer);
    CrwBuffer* pPrevCrwBuffer(void);
    void       vPrevCrwBuffer(CrwBuffer* pPrevBuffer);
    void       vPrintRows(StreamDef* pStreamDef);
    CsvBuffer* pToCsvBuffer(void);
    //SrtBuffer* pToSrtBuffer(i8 iKeyCol);
    void       vTruncate(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // row functions
    Crw*       aCrw(i8 i_Row);
    void       vGetRow(RowSingle* pRow, i8 i_Row);
    void       vGetRow(RowSlab* pRowSlab, i8 i_Row);
    void       vShow(void);
    void       vShowRow(i8 i_Row);
    StreamDef* pStreamDef(void);

    ////////////////////////////////////////////////////////////////////////////
    // row functions
    void vAppendClose(void);
    void vAppendOpen(RowCompactor* pRowCompactor, bool bCollectSamples);
    bool bAppendCrw(Crw* aCrwSrc);
    bool bAppendRsb(Row* aRowSrc);
    bool bAppendSrw(RowSingle* pRowSingle);

  private:
    bool bHasSpace(i8 nBytes);


    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertCrwBuffer(void);
    bool bValidCrwBuffer(void);
  
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void vAnalyze(i8& nBuffers, i8& nRows);
    void vDump(void);
    void vRestore(void);
    void vRestoreNub(void);
    void vRestoreTable(Table* pTable);
    bool bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions        
    bool bVerifyNextPrev(void);
    bool bVerifyRows(void);
    void vVerifyFault(const char* azFunc, const char* azFilePath, i8 iLineNo);

    public:
    Footer* aFooter(void);
  
  
    /*----------------------------------------------------------------------------*/
    /* check if the row buffer contains the given address                         */
    /*----------------------------------------------------------------------------*/
    inline static bool bCheckAddress(CrwBuffer* pCrwBuffer, void* aAddr)
    {
      PoolBuffer* pRowPoolBuffer  = PoolBuffer::pWithinPoolBuffer(pCrwBuffer);
      PoolBuffer* pAddrPoolBuffer = PoolBuffer::pWithinPoolBuffer((Object*)aAddr);
    
      if (pAddrPoolBuffer == pRowPoolBuffer)
        return true;
      else
        return false;
    }
  
  };
  
  CrwBuffer* _pCrwBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
