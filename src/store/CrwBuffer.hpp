/*============================================================================*/
/* CrwBuffer.hpp -- Compact Row Buffer                                        */
/*============================================================================*/
#pragma once

namespace STORE { class Tub; }

#include "RowSingle.hpp"
#include "Crw.hpp"
#include "Tub.hpp"
#include "Pot.hpp"

namespace STORE
{
  class CrwBuffer : public DataBuffer
  {
  private:
    RowTray    m_oRowTray;  // tray of rows
    SeeMe      m_xSeeMe;
    RowCompactor* m_pRowCompactor;
  
  
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
    static void       vRepair(Object* pObject);
    static CrwBuffer* pToCrwBuffer(void* aAddr);
    static void       vToRow(RowSingle* pRow, Crw* aCrw);
  
    static void       vPostRecv(void* aCrwBuffer);

    void       vPtnToBkt(LglPtn tLglPtn);
    void       vStreamDef(StreamDef* pStreamDef);
    void       vTable(Table* pTable);

    RowCompactor* pRowCompactor(void);;
    RowTray* pRowTray(void);;
  
    void       vDelete(Crw* aCrw);
    HeadLine*  aGetHead(i8 i_Crw);
    void       vOperandDump(i8 iDetail);
    i8         nRows(void);
    i8         nRowSize(void);
    i8         nAllotedBytes(void);
    i8         jRowSizeEst(void);
    i8         nHeadSize(void);


    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
    void       vCopy(CrwBuffer* pArgCrwBuffer);
    i8         iDecile(void);
    void       vInflateRows(StreamDef* pStreamDef);
    void       vInflateRows(Tub* pTub);
    void       vPrintRows(StreamDef* pStreamDef);
    void       vToCsv(Tub* pTub);
    void       vToRowRef(Tub* pTub, i8 iKey);
    //SrtBuffer* pToSrtBuffer(i8 iKeyCol);
    
    ////////////////////////////////////////////////////////////////////////////
    // row functions
    Crw*       aCrw(i8 i_Row);
    void       vGetRow_single(RowSingle* pRowSingle, i8 i_Row);
    void       vShow(void);
    void       vShowRow(i8 i_Row);

    ////////////////////////////////////////////////////////////////////////////
    // row functions
    void vAppendClose(void);
    void vAppendOpen(RowCompactor* pRowCompactor, bool bCollectSamples);
    bool bAppendCrw(Crw* aCrwSrc);
    bool bAppendRow(Row* aRowSrc);

  private:
    bool bHasSpace(i8 nBytes);


    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertCrwBuffer(void);
    bool bValidCrwBuffer(void);
  
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions

    void vDump(void);
    void vRestoreNub(void);
    bool bVerifyRows(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
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
