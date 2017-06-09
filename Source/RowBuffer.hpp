/*============================================================================*/
/* RowBuffer.hpp -- Entry Buffer                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  static const i8 C_BUFFER_THRESHOLD = 100;
  static const RowBuffer* C_ROWBUFFER_MARKER = (RowBuffer*)0xFFFFFFFFFFFFFFFF;
  
  
  enum eRD // Entry Buffer Durability
  {
  /* 0 */ RD_NONE=0,
  /* 1 */ RD_TEMPORARY,     // rb is temporary
  /* 2 */ RD_PERMANENT,     // rb is permanent
  /* 3 */ RD_ENUMS
  };
  
  static Str* aRD[]
  {
  /* 0 */ "NONE"_Str,
  /* 1 */ "TEMPORARY"_Str,
  /* 2 */ "PERMANENT"_Str,
  /* 3 */ "ENUMS"_Str,
  };
  
  class RowBuffer : public Buffer
  {
  private:
    eRD          m_eDurability; // row buffer durability
    i8           m_nRows;       // number of rows in this buffer
    RowBuffer*   m_pPrevBuffer; // prev buffer
    RowBuffer*   m_pNextBuffer; // next buffer
    WspFxd<C_1M> m_xWsp;        // ..data.. rows
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
    
    RowBuffer(void);
    RowBuffer(i8 iDataBytes);
    ~RowBuffer(void);
    
    static RowBuffer* pNewRowBuffer(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
  
    Entry*       pAllocRow(i8 nDataBytes);
    void       vAppendBuffer(RowBuffer* pSrcBuffer);
    void       vCopyBuffer(RowBuffer* pSrcBuffer);
    byte*      aDataArea(void);
    eRD        eDurability(void);
    void       vDurability(eRD eDurability);
    Entry*       pFirstRow(void);
    i8         nFreeBytes(void);
    bool       bInsertRow(TxRef* pTxRef, Str* pRecord, i8 iLength, Entry** ppRow);
    RowBuffer* pNextBuffer(void);
    void       vNextBuffer(RowBuffer* pNextBuffer);
    Entry*       pNextRow(Entry* pCurrRow);
    RowBuffer* pPrevBuffer(void);
    void       vPrevBuffer(RowBuffer* pPrevBuffer);
    i8         nRows(void);
    void       vRowCount(void);
    i8         nUsedBytes(void);
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
